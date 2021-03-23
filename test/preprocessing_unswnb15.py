#!/usr/bin/env python
# coding: utf-8

import argparse
import pandas as pd
from helpers import *
pd.options.mode.use_inf_as_na = True


def preprocess_unswnb15(TRAIN_DATA, TEST_DATA, generate_labels_file=False,
        multi_class=False):
    # df = pd.read_csv("UNSW_NB15_training-set.csv")
    # df_test = pd.read_csv("UNSW_NB15_testing-set.csv")
    df = pd.read_csv(TRAIN_DATA)
    df_test = pd.read_csv(TEST_DATA)

    try:
        df.drop('id', inplace=True, axis=1)
    except:
        print("treino não tem id")

    try:
        df_test.drop('id', inplace=True, axis=1)
    except:
        print("teste não tem id")

    if multi_class:
        try:
            df.drop('label', inplace=True, axis=1)
        except:
            print("treino não tem label")

        try:
            df_test.drop('label', inplace=True, axis=1)
        except:
            print("teste não tem label")


    cols = df.columns.tolist()
    cols = cols[-1:] + cols[:-1]
    df = df[cols]
    df_test = df_test[cols]
    df['service'].replace('-', 'unidentified', inplace=True)
    df_test['service'].replace('-', 'unidentified', inplace=True)
    data = dict((i[1], i[0]) for i in enumerate(df.proto.unique(), start=1))
    services = dict((i[1], i[0]) for i in enumerate(df.service.unique(), start=1))
    states = dict((i[1], i[0]) for i in enumerate(set(list(df.state.unique()) + list(df_test.state.unique())),start=1))
    attack_cat  = dict((i[1], i[0]) for i in enumerate(set(list(df.attack_cat.unique()) + list(df_test.attack_cat.unique())),start=1))

    df.label = pd.factorize(df['label'])[0] + 1
    df_test.label = pd.factorize(df_test['label'])[0] + 1

    for i in data.keys():
        df.loc[df['proto'].eq(i), 'proto'] = data[i]
        df_test.loc[df_test['proto'].eq(i),'proto'] = data[i]

    df['proto'] = df['proto'].astype('int64')
    df_test['proto'] = df_test['proto'].astype('int64')


    for i in services.keys():
        df.loc[df['service'].eq(i), 'service'] = services[i]
        df_test.loc[df_test['service'].eq(i),'service'] = services[i]

    df['service'] = df['service'].astype('int64')
    df_test['service'] = df_test['service'].astype('int64')


    for i in states.keys():
        df.loc[df['state'].eq(i), 'state'] = states[i]
        df_test.loc[df_test['state'].eq(i),'state'] = states[i]

    df['state'] = df['state'].astype('int64')
    df_test['state'] = df_test['state'].astype('int64')


    for i in attack_cat.keys():
        df.loc[df['attack_cat'].eq(i), 'attack_cat'] = attack_cat[i]
        df_test.loc[df_test['attack_cat'].eq(i),'attack_cat'] = attack_cat[i]

    df['attack_cat'] = df['attack_cat'].astype('int64')
    df_test['attack_cat'] = df_test['attack_cat'].astype('int64')

    if generate_labels_file and not multi_class:
        with open("Labels.txt", 'w') as f:
            f.write("Labels Meaning\n")
            for k,v in attack_cat.items():
                f.write(f"{k} - {v}\n")

    return df, df_test


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="Normalize UNSW to work with FEMaFS and OPF")
    requiredNamed = parser.add_argument_group('required named arguments')
    requiredNamed.add_argument('-ta', '--train', nargs=1, type=str, action='store',
                               help="<Required> UNSW NB15 train data ",
                               required=True)
    requiredNamed.add_argument('-te', '--test', nargs=1,
                               help="<Required> UNSW NB15 test data ",
                               action='store', type=str, required=True)
    args = parser.parse_args()
    if not args:
        print(parser.print_help)

    if args.train and args.test :
        df, df_test = preprocess_unswnb15(args.train[0], args.test[0],
                generate_labels_file=True)
        df_test_sampled = df_test.sample(frac=0.1)
        df_sampled = df.sample(frac=0.1)
        generate_file(df_test_sampled, 'opf', 'test-unsw-nb15', log1p=True)
        generate_file(df_sampled, 'opf', 'train-unsw-nb15', log1p=True)
        generate_file(df_test_sampled, 'fem', 'test-unsw-nb15', log1p=True)
        generate_file(df_sampled, 'fem', 'train-unsw-nb15', log1p=True)
        # generate_file(df, 'opf', 'train-unsw-nb15')
        # generate_file(df_test, 'opf', 'test-unsw-nb15')

    else:
        print(parser.print_help)
