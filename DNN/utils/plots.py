import os
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import tensorflow as tf
from sklearn.metrics import confusion_matrix
from sklearn.utils.multiclass import unique_labels

def plot_confusion_matrix(y_true, y_pred, classes, normalize=False,  title=None, cmap=plt.cm.Blues, savename="./cm.pdf"):
    # This function prints and plots the confusion matrix. Normalization can be applied by setting `normalize=True`.
    if not title:
        if normalize:
            title = 'Normalized confusion matrix'
        else:
            title = 'Confusion matrix, without normalization'

    # Compute confusion matrix
    cm = confusion_matrix(y_true, y_pred)
    # Only use the labels that appear in the data
    #classes = classes[unique_labels(y_true, y_pred)]
    if normalize:
        cm = cm.astype('float') / cm.sum(axis=1)[:, np.newaxis]
        print("Normalized confusion matrix")
    else:
        print('Confusion matrix, without normalization')

    print(cm)

    fig, ax = plt.subplots()
    im = ax.imshow(cm, interpolation='nearest', cmap=cmap)
    ax.figure.colorbar(im, ax=ax)
    ax.set(xticks=np.arange(cm.shape[1]), yticks=np.arange(cm.shape[0]), xticklabels=classes, yticklabels=classes,
           title=title, ylabel='True label', xlabel='Predicted label')

    # Rotate the tick labels and set their alignment.
    plt.setp(ax.get_xticklabels(), rotation=45, ha="right", rotation_mode="anchor")

    # Loop over data dimensions and create text annotations.
    fmt = '.2f' if normalize else 'd'
    thresh = cm.max() / 2.
    for i in range(cm.shape[0]):
        for j in range(cm.shape[1]):
            ax.text(j, i, format(cm[i, j], fmt), ha="center", va="center",
                    color="white" if cm[i, j] > thresh else "black")
    fig.tight_layout()

    plt.savefig(savename)
    plt.gcf().clear()
    return ax

def plot_performance(hist, savedir="./"):
    print("Plotting scores")
    plt.plot(hist.history['sparse_categorical_accuracy'])
    plt.plot(hist.history['val_sparse_categorical_accuracy'])
    plt.title('Model accuracy')
    plt.ylabel('Accuracy')
    plt.xlabel('Epoch')
    plt.legend(['Train','Test'], loc='lower right')
    plt.savefig(os.path.join(savedir+'/fig_score_acc.pdf'))
    plt.gcf().clear()

    plt.plot(hist.history['loss'])
    plt.plot(hist.history['val_loss'])
    plt.ylabel('Loss')
    plt.xlabel('Epoch')
    plt.legend(['Train','Test'],loc='upper right')
    plt.savefig(os.path.join(savedir+'/fig_score_loss.pdf'))
    plt.gcf().clear()

def plot_output_dist(train, test, savedir="./"):
    sigtrain = np.array(train[train["True"]==1]["Pred"])
    bkgtrain = np.array(train[train["True"]==0]["Pred"])
    sigtest = np.array(test[test["True"]==1]["Pred"])
    bkgtest = np.array(test[test["True"]==0]["Pred"])
    bins=40
    scores = [sigtrain, sigtest, bkgtrain, bkgtest]
    #print (scores)
    low = min(np.min(d) for d in scores)
    high = max(np.max(d) for d in scores)

    # test is dotted
    plt.hist(sigtrain, color="b", alpha=0.5, range=(low, high), bins=bins, histtype="stepfilled", density=True, label="sig train")
    plt.hist(bkgtrain, color="r", alpha=0.5, range=(low, high), bins=bins, histtype="stepfilled", density=True, label="bkg train")
    # train is filled
#    plt.hist(sigtest, color="b", range=(low, high), bins=bins, histtype="step", density=True, label="sig test")
#    plt.hist(bkgtest, color="r", range=(low, high), bins=bins, histtype="step", density=True, label="bkg test")
        
    hist, bins = np.histogram(sigtest, bins=bins, range=(low,high), density=True)
    scale = len(sigtest) / sum(hist)
    err = np.sqrt(hist * scale) / scale
    width = (bins[1] - bins[0])
    center = (bins[:-1] + bins[1:]) / 2
    plt.errorbar(center, hist, yerr=err, fmt='o', c='b', label='S (test)')
    hist, bins = np.histogram(bkgtest, bins=bins, range=(low,high), density=True)
    scale = len(bkgtest) / sum(hist)
    err = np.sqrt(hist * scale) / scale
    plt.errorbar(center, hist, yerr=err, fmt='o', c='r', label='B (test)')

    plt.title("Output distribution")
    plt.ylabel("entry")
    plt.xlabel("probability")
    plt.legend(loc='best')
    plt.savefig(os.path.join(savedir+'/fig_output_dist.pdf'))
    plt.gcf().clear()

def plot_corrMatrix(dataframe, savedir="./", outname=""):
    corrdf = dataframe.corr()
    fig, ax1 = plt.subplots(ncols=1, figsize=(10,9))

    opts = {'cmap': plt.get_cmap("RdBu"),
            'vmin': -1, 'vmax': +1}
    heatmap1 = ax1.pcolor(corrdf, **opts)
    plt.colorbar(heatmap1, ax=ax1)

    labels = corrdf.columns.values
    for ax in (ax1,):
        ax.tick_params(labelsize=8)
        # shift location of ticks to center of the bins
        ax.set_xticks(np.arange(len(labels))+0.5, minor=False)
        ax.set_yticks(np.arange(len(labels))+0.5, minor=False)
        ax.set_xticklabels(labels, minor=False, ha='right', rotation=90)
        ax.set_yticklabels(labels, minor=False)
    
    plt.tight_layout()
    
    plt.savefig(os.path.join(savedir+'/correlation_'+outname+'.pdf'))
    plt.gcf().clear()

def plot_roc_curve(fpr,tpr,auc,savedir="./"): 
    plt.plot(fpr,tpr) 
    plt.axis([0,1,0,1])
    plt.title('AUC = '+str(auc))
    plt.xlabel('False Positive Rate') 
    plt.ylabel('True Positive Rate')
    plt.tight_layout()
    plt.savefig(os.path.join(savedir+'/fig_roc.pdf'))
    plt.gcf().clear()
