import pandas as pd
import numpy as np
import os
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import MinMaxScaler, StandardScaler
from sklearn.feature_selection import VarianceThreshold
pd.options.mode.use_inf_as_na = True

def normalize(df, log1p=False, grayNorm=False):
    result = df.copy()
    result.fillna(0)
    if grayNorm:
        result[result.columns[1:]] = result[result.columns[1:]].apply(lambda x: ((x-x.min())/(x.max() - x.min())) * 255)
    if log1p:
        result[result.columns[1:]] = result[result.columns[1:]].apply(lambda x: np.log1p(x - x.min() + 1))

    result[result.columns[1:]] = MinMaxScaler().fit_transform(result[result.columns[1:]].to_numpy())
    return result


def low_variance_threshold(df, n_var):
    sel = VarianceThreshold(threshold=(n_var * (1 - n_var) ))
    sel_var=sel.fit_transform(df)
    result_high_var = df[df.columns[sel.get_support(indices=True)]]
    removed_cols = [x for x in df.columns if x not in result_high_var.columns]
    print(f"removida {len(removed_cols)} features/colunas, sendo elas ", removed_cols)
    return result_high_var


def train_test_files(train, test, name, _type, cls):
    TRAIN = f'temp-train-{name}'
    TEST = f'temp-test-{name}'
    if _type == 'opf':
        train.to_csv(TRAIN, sep=" ")
        test.to_csv(TEST, sep=" ")
        FILE_TRAIN=f'train-{name}.opf'
        FILE_TEST=f'test-{name}.opf'
    else:
        train.to_csv(TRAIN, sep=" ", index=False)
        test.to_csv(TEST, sep=" ", index=False)
        FILE_TRAIN=f'train-{name}.fem'
        FILE_TEST=f'test-{name}.fem'

    with open(TRAIN, 'r') as CSV:
        samples, features  = train.shape
        with open(FILE_TRAIN, "w") as f:
            if _type == 'opf':
                f.write(f"{samples} {cls} {features-1}\n")
            else:
                f.write(f"{cls} {samples} {features-1}\n")
            f.writelines(CSV.readlines()[1:])
            print(f"Criado arquivo de treino {FILE_TRAIN} com {cls} classes, {samples} amostras e {features-1} features")

    with open(TEST, 'r') as CSV:
        samples, features  = test.shape
        with open(FILE_TEST, "w") as f:
            if _type == 'opf':
                f.write(f"{samples} {cls} {features-1}\n")
            else:
                f.write(f"{cls} {samples} {features-1}\n")

            f.writelines(CSV.readlines()[1:])
            print(f"Criado arquivo de treino {FILE_TEST} com {cls} classes, {samples} amostras e {features-1} features")

    os.remove(TRAIN)
    os.remove(TEST)

def generate_file(df, _type, name, create_train_test=False, cls=None, _test_size=0.30, low_variance_remove=False,
                  threshold_n = 0.95, log1p=False):
    df_data = df.copy()
    df_data.reset_index(inplace=True, drop=True)
    if low_variance_remove:
        print(f"Removendo colunas com baixa variança... threshold de {threshold_n * (1 - threshold_n)}")
        df_data = low_variance_threshold(df_data, threshold_n)

    if not cls:
        cls = len(df.iloc[:,0].unique())
    df_data = normalize(df_data, log1p)

    df_data = df_data.round(7)



    if _type == 'opf':
        FILE = f'{name}.opf'
        df_data.to_csv(name, sep=" ")
    else:
        FILE = f'{name}.fem'
        df_data.to_csv(name, sep=" ", index=False)

    samples, features  = df_data.shape
    with open(name, "r") as CSV:
        with open(FILE, "w") as f:
            if _type == 'opf':
                f.write(f"{samples} {cls} {features-1}\n")
            else:
                f.write(f"{cls} {samples} {features-1}\n")
            f.writelines(CSV.readlines()[1:])

    if create_train_test:
        print(f"Gerando split de treino / test , split com Treino {(1-_test_size) * 100 }%, Teste {_test_size * 100}%")
        train, test = train_test_split(df_data, test_size=_test_size)
        train.reset_index(inplace=True, drop=True)
        test.reset_index(inplace=True, drop=True)
        #return train,test
        train_test_files(train,test, name, _type, cls)

    os.remove(name)
    if create_train_test:
        print(f"Arquivo sem split {FILE} salvado com {cls} classes, {samples} amostras e {features-1} caracteristicas")
    else:
        print(f"Arquivo {FILE} salvado com {cls} classes, {samples} amostras e {features-1} caracteristicas")
