import os
import sys

#os.environ["CUDA_DEVICE_ORDER"] = "PCI_BUS_ID"
os.environ["CUDA_VISIBLE_DEVICES"] = "1"

import uproot
import pandas as pd
import numpy as np
import tensorflow as tf
import matplotlib.pyplot as plt
import pickle
from keras.callbacks import EarlyStopping, ModelCheckpoint
from utils.plots import *
from sklearn.preprocessing import MinMaxScaler
from sklearn.metrics import roc_curve, roc_auc_score

root_dir = os.getcwd().replace("DNN","") # Upper directory
# MODIFY !!!
syst = "nom"
label = "rerun"
class_names = ["sig", "bkg"]

# Calculating AUC (metric function)
#def auroc(y_true, y_pred):
#    return tf.py_func(roc_auc_score, (y_true, y_pred[:,1]), tf.double)

for p in ["ST","TT"]:
    processed = "aug22_"+p.lower()+"lfv"
    print("Start "+p+" LFV Training")
    epochs = 1000
    inputvars = []
    if p == "ST":
        inputvars = ["Sel_muon1pt","Sel_muon1eta",
            "Sel_tau1pt","Sel_tau1eta","Sel_tau1mass",
            "Sel2_jet1pt","Sel2_jet2pt","Sel2_jet3pt",
            "Sel2_jet1eta","Sel2_jet2eta","Sel2_jet3eta",
            "Sel2_jet1mass","Sel2_jet2mass","Sel2_jet3mass",
            "Sel2_jet1btag","Sel2_jet2btag","Sel2_jet3btag",
            "Sys_METpt","Sys_METphi",
            "chi2","chi2_SMW_mass","chi2_SMTop_mass",
            "chi2_wqq_dEta","chi2_wqq_dPhi","chi2_wqq_dR",
            "mutau_mass","mutau_dEta","mutau_dPhi","mutau_dR",
            ]
        sbratio = 1 # sig:bkg = 1:1
    elif p == "TT":
        inputvars = ["Sel_muon1pt","Sel_muon1eta",
            "Sel_tau1pt","Sel_tau1eta","Sel_tau1mass",
            "Sel2_jet1pt","Sel2_jet2pt","Sel2_jet3pt","Sel2_jet4pt",
            "Sel2_jet1eta","Sel2_jet2eta","Sel2_jet3eta","Sel2_jet4eta",
            "Sel2_jet1mass","Sel2_jet2mass","Sel2_jet3mass","Sel2_jet4mass",
            "Sel2_jet1btag","Sel2_jet2btag","Sel2_jet3btag","Sel2_jet4btag",
            "Sys_METpt","Sys_METphi",
            "chi2","chi2_lfvTop_mass","chi2_SMW_mass","chi2_SMTop_mass",
            "chi2_wqq_dEta","chi2_wqq_dPhi","chi2_wqq_dR",
            "chi2_lfvjmu_dEta","chi2_lfvjmu_dPhi","chi2_lfvjmu_dR","chi2_lfvjmu_mass",
            "chi2_lfvjtau_dEta","chi2_lfvjtau_dPhi","chi2_lfvjtau_dR","chi2_lfvjtau_mass",
            "chi2_lfvjmutau_dEta","chi2_lfvjmutau_dPhi","chi2_lfvjmutau_dR","chi2_lfvjmutau_mass",
            "mutau_mass","mutau_dEta","mutau_dPhi","mutau_dR",
            ]
        sbratio = 1 # sig:bkg = 1:1

    project_dir = "nanoaodframe/"+processed+"/"+syst+"/"
    sig_filedir = root_dir+project_dir+p+"_LFV_nom.root"
    bkg1_filedir = root_dir+project_dir+"TTTo2L2Nu_nom.root"
    bkg2_filedir = root_dir+project_dir+"TTToSemiLeptonic_nom.root"
    train_outdir = label+"_"+processed+"/"+syst
    os.makedirs(train_outdir, exist_ok=True)

    sig_tree = uproot.open(sig_filedir)["outputTree2"]
    bkg1_tree = uproot.open(bkg1_filedir)["outputTree2"]
    bkg2_tree = uproot.open(bkg2_filedir)["outputTree2"]

    df_sig = sig_tree.arrays(inputvars,library="pd")
    df_bkg1 = bkg1_tree.arrays(inputvars,library="pd")
    df_bkg2 = bkg2_tree.arrays(inputvars,library="pd")
    df_bkg = pd.concat([df_bkg1,df_bkg2])

    ntotsig = len(df_sig)
    ntotbkg = len(df_bkg)

    nsig = ntotsig
    nbkg = ntotbkg

    if nsig >= nbkg:
        if sbratio == 1:
            nsig = nbkg
        elif sbratio == 2:
            nsig = int(nbkg/2)
    else:
        if sbratio == 1:
            nbkg = nsig
        elif nsig>=int(nbkg/2):
            if sbratio == 2:
                nsig = int(nbkg/2)
        else:
            "Error::Check the number of events!"
            sys.exit()

    print("LFV : "+str(nsig)+" events")
    print("TT  : "+str(nbkg)+" events")
    df_sig = df_sig.sample(n=nsig)
    df_bkg = df_bkg.sample(n=nbkg)
    df_sig["category"] = 1
    df_bkg["category"] = 0

    pd_data = pd.concat([df_sig,df_bkg])
    colnames = pd_data.columns
    pd_sig = pd_data[pd_data['category'] == 1]
    pd_bkg = pd_data[pd_data['category'] == 0]
    plot_corrMatrix(pd_data,train_outdir,"total")
    plot_corrMatrix(pd_sig,train_outdir,"sig")
    plot_corrMatrix(pd_bkg,train_outdir,"bkg")

    pd_data = pd_data.sample(frac=1).reset_index(drop=True)

#    # Min-Max Scale
#    scaler = MinMaxScaler()
#    pd_data = pd.DataFrame(scaler.fit_transform(pd_data), columns=pd_data.columns)

    x_total = np.array(pd_data.filter(items = inputvars))
    y_total = np.array(pd_data.filter(items = ['category']))

    
    # Splitting between training set and cross-validation set
    numbertr = len(y_total)
    trainlen = int(0.7*numbertr) # Fraction used for training
    x_train = x_total[:trainlen,0::]
    y_train = y_total[:trainlen]
    x_val = x_total[trainlen:,0::]
    y_val = y_total[trainlen:]

    patience_epoch = 30
#    if p == "TT":
#        patience_epoch = 50
    # Early Stopping with Validation Loss for Best Model
    es = EarlyStopping(monitor='val_loss', mode='min', verbose=1, patience=patience_epoch)
    mc = ModelCheckpoint(train_outdir+'/best_model.h5', monitor='val_loss', mode='min', save_best_only=True)

    ###################################################
    #                      Model                      #
    ###################################################
    model = tf.keras.models.Sequential()

    ###############    Input Layer      ###############
    model.add(tf.keras.layers.Flatten(input_shape = (x_train.shape[1],)))

    #drop = 0.2
    activation_function='relu'
    #activation_function='elu'
    weight_initializer = 'random_normal'
    #weight_initializer = 'he_uniform'
    ###############   Hidden Layer 1    ###############
    model.add(tf.keras.layers.BatchNormalization())
    model.add(tf.keras.layers.Dense(50, activation=activation_function, kernel_regularizer='l2', kernel_initializer=weight_initializer))
    #model.add(tf.keras.layers.Dropout(drop))

    ###############   Hidden Layer 2    ###############
    model.add(tf.keras.layers.BatchNormalization())
    model.add(tf.keras.layers.Dense(50, activation=activation_function, kernel_regularizer='l2', kernel_initializer=weight_initializer))
    #model.add(tf.keras.layers.Dropout(drop))
    
#    ###############   Hidden Layer 3    ###############
#    model.add(tf.keras.layers.BatchNormalization())
#    model.add(tf.keras.layers.Dense(50, activation=activation_function, kernel_regularizer='l2', kernel_initializer=weight_initializer))
#    model.add(tf.keras.layers.Dropout(drop))
#    
#    ###############   Hidden Layer 4    ###############
#    model.add(tf.keras.layers.BatchNormalization())
#    model.add(tf.keras.layers.Dense(30, activation=activation_function, kernel_regularizer='l2', kernel_initializer=weight_initializer))
#    #model.add(tf.keras.layers.Dropout(drop))
    
    ###############    Output Layer     ###############
    model.add(tf.keras.layers.Dense(2, activation=tf.nn.softmax))
    batch_size = 1024

    model.compile(loss='sparse_categorical_crossentropy',
                  optimizer = 'adam',
                  #optimizer = 'Adadelta',
                  #optimizer = 'Nadam',
                  #optimizer = 'Adamax',
                  metrics=['accuracy', 'sparse_categorical_accuracy'])

    hist = model.fit(x_train, y_train, batch_size=batch_size, epochs=epochs, 
                                    validation_data=(x_val,y_val), callbacks=[es, mc])

    #tf.keras.models.save_model(model,'model_{epoch:%d}.h5', overwrite=True, include_optimizer=True)
    print(hist.history.keys())
    model.summary()

    pred_train = model.predict(x_train)
    train_result = pd.DataFrame(np.array([y_train.T[0], pred_train.T[1]]).T, columns=["True", "Pred"])
    pred_val = model.predict(x_val)
    val_result = pd.DataFrame(np.array([y_val.T[0], pred_val.T[1]]).T, columns=["True", "Pred"])

    pred = np.argmax(pred_val, axis=1)
   
    fpr, tpr, thresholds = roc_curve(y_val,pred_val[:,1])
    auc = roc_auc_score(y_val,pred_val[:,1])
    #pred = np.argmax(model.predict(valid_data), axis=1)
    comp = np.reshape(y_val,(-1))

    acc = 100 * np.mean( pred == comp )

    plot_confusion_matrix(comp, pred, classes=class_names,
                        title='Confusion matrix, without normalization, acc=%.2f'%acc, savename=train_outdir+"/confusion_matrix.pdf")

    plot_confusion_matrix(comp, pred, classes=class_names, normalize=True,
                        title='Normalized confusion matrix, acc=%.2f'%acc, savename=train_outdir+"/norm_confusion_matrix.pdf")

    plot_performance(hist=hist, savedir=train_outdir)

    plot_output_dist(train_result, val_result, savedir=train_outdir)

    plot_roc_curve(fpr, tpr, auc, savedir=train_outdir)

    l = [fpr.tolist(),tpr.tolist()]
    with open(train_outdir+"/roc_list.pkl", 'wb') as f:
        pickle.dump(l, f)
