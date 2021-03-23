import os
import sh
import pandas as pd
import numpy as np

from sklearn.metrics import accuracy_score
from sklearn.metrics import f1_score
from sklearn.metrics import confusion_matrix
from sklearn.metrics import balanced_accuracy_score
from sklearn.metrics import jaccard_score
from sklearn.metrics import roc_auc_score
from sklearn.metrics import matthews_corrcoef


def generate_dict_data(fem:str, data:list, fix:str=None):
    if fix:
        files = sorted([ name for name in os.listdir(os.getcwd()) if
            os.path.isfile(os.path.join(os.getcwd(), name))
            and (fem in name and name.startswith(fix))])
    else:
        files = sorted([ name for name in os.listdir(os.getcwd()) if
            os.path.isfile(os.path.join(os.getcwd(), name)) and fem in name])

    for f in files:
        df = pd.read_csv(f, sep=" ", header=None)
        line = f.replace("-", "").split("_")
        if len(line) > 5:
            method = line[0]
            iteration = line[2].replace("-", "")
            dataset = line[3]
            percentage = line[4][:-1]

        else:
            method = line[0]
            iteration = line[1]
            dataset = line[2].replace("-", "")
            percentage = line[3][:-1]

        dline = {"method" : method,
                 "fem":fem,
                 "iteration" : iteration,
                 "dataset" : dataset,
                 "percentage": percentage,
                 "f1_score": f1_score(df[0], df[1], average='weighted'),
                 'jaccard':jaccard_score(df[0], df[1], average='weighted'),
                 'accuracy':accuracy_score(df[0], df[1]),
                 'MCC': matthews_corrcoef(df[0], df[1]),
                 'balanced_accuracy':balanced_accuracy_score(df[0], df[1]),
                 }
        # if dataset == 'tornontor':
             # print(dline)
        data.append(dline)



def walk_through_testdir(directory:str, finaldata:list):
    os.chdir(directory)
    print(os.getcwd())
    dset, perc, fem = directory.split('_')[1:]
    if "tornontor" == dset:
        generate_fema_opf_pd(dset, perc,fem, 'tor-nontor')
        generate_opf_pd(dset,perc,fem)
        generate_dict_data(fem, finaldata, 'fema')
        generate_dict_data(fem, finaldata, 'opf')
    elif "unswnb15" == dset:
        generate_fema_opf_pd(dset, perc,fem, 'usnw-nb15')
        generate_opf_pd(dset,perc,fem)
        generate_dict_data(fem, finaldata, 'fema')
        generate_dict_data(fem, finaldata, 'opf')

    else:
        generate_fema_opf_pd(dset, perc,fem)
        generate_opf_pd(dset,perc,fem)
        generate_dict_data(fem, finaldata)

    os.chdir("..")


def generate_fema_opf_pd(dataset:str, percentage:str, fem:str,
        fixdatasetname:str=None):

    testdirs=sorted([ name for name in os.listdir(os.getcwd()) if os.path.isdir(os.path.join(os.getcwd(), name))])
    for d in testdirs:
        try:
           i = d.split("_")[-1]
           if fixdatasetname:
               out_file = f'fema_opf_{i}_{fixdatasetname}_{percentage.lower()}_{fem}'
               pr_file=f'iteration_{i}/test_{fixdatasetname.lower()}_{percentage.lower()}.dat.out'
           else:
               out_file = f'fema_opf_{i}_{dataset}_{percentage.lower()}_{fem}'
               pr_file=f'iteration_{i}/test_{dataset.lower()}_{percentage.lower()}.dat.out'

           sh.cut(sh.tail(f"iteration_{i}/test.feature.out", n="+2"), f=2, d=" ", _out=f"fema_{i}")
           sh.pr(f'fema_{i}', pr_file, w=3,m=True,t=True, _out=out_file)
           sh.rm(f'fema_{i}', f=True)

        except Exception as e:
           print(f"Error processing data from {d}, {dataset}, {percentage},{fem}")
           print(e)
        except:
           print(f"Error processing data from {d}, {dataset}, {percentage},{fem}")


def generate_opf_pd(dataset:str, percentage:str, fem:str,
        fixdatasetname:str=None):
    testdirs=sorted([ name for name in os.listdir(os.getcwd()) if os.path.isdir(os.path.join(os.getcwd(), name))])
    for d in testdirs:
        try:
           i = d.split("_")[-1]
           pr_file=f'iteration_{i}/testing.dat.out'
           if fixdatasetname:
               tail_file = f'test-{fixdatasetname}.opf'
               out_file = f'opf_{i}_{fixdatasetname}_{percentage.lower()}_{fem}'
           else:
               tail_file = f'test-{dataset}.opf'
               out_file = f'opf_{i}_{dataset}_{percentage.lower()}_{fem}'

           sh.cut(sh.tail(f"iteration_{i}/{tail_file}", n="+2"),
                   f=2, d=" ", _out=f"opf_{i}")
           sh.pr(f'opf_{i}', pr_file,
                   w=3,m=True,t=True, _out=out_file)
           sh.rm(f'opf_{i}')

        except Exception as e:
           print(f"Error processing data from {d}, {dataset}, {percentage},{fem}")
           print(e)
        except:
           print(f"Error processing data from {d}, {dataset}, {percentage},{fem}")





def walk_through_datasets(startdir:str, dataset:list):
    os.chdir(startdir)
    pandaslist = []
    for d in dataset:
        print(f"Walking through dataset {d} tests")
        dirs = sorted([ name for name in os.listdir(startdir) if
            os.path.isdir(os.path.join(startdir, name)) and d in name])
        for i in dirs:
            walk_through_testdir(i, finaldata=pandaslist)
        print(f"Done with dataset")
    df = pd.DataFrame(data=pandaslist)
    df['dataset'] = df['dataset'].apply(lambda x: x.lower().replace('-', ''))
    print(f"Generating final_data.csv in {startdir}")
    df.to_csv("final_data.csv", index=False)
    print(f"File Generated")

if '__name__' == __main__:
    datasets = [
        'nslkdd',
        'tornontor',
        'UNSWNB15'
        ]
    walk_through_datasets("${CMAKE_CURRENT_BINARY_DIR}", datasets)
