#!/usr/bin/env python
# coding: utf-8

import argparse
import pandas as pd
from helpers import *
pd.options.mode.use_inf_as_na = True

def preprocessing(df) :
    df.fillna(0, inplace=True)
    df_sampled = df.sample(frac=0.1)
    train, test = train_test_split(df_sampled, test_size=0.30)
    train.reset_index(inplace=True, drop=True)
    test.reset_index(inplace=True, drop=True)
    return train,test

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="Normalize UNSW to work with FEMaFS and OPF")
    requiredNamed = parser.add_argument_group('required named arguments')
    requiredNamed.add_argument('-ta', '--train', nargs=1, type=str, action='store',
            help="<Required> UNSW NB15 train data ",
            required=True)
    requiredNamed.add_argument('-te', '--test', nargs=1,
            help="<Required> UNSW NB15 test data ",
            action='store', type=str, required=False)

    args = parser.parse_args()
    if not args:
        print(parser.print_help)

    if args.train:
        df = pd.read_csv(args.train[0])
        df_train_sampled, df_test_sampled = preprocessing(df)
        generate_file(df_test_sampled, 'opf', 'test-cicids2017', log1p=True)
        generate_file(df_train_sampled, 'opf', 'train-cicids2017', log1p=True)
        generate_file(df_test_sampled, 'fem', 'test-cicids2017', log1p=True)
        generate_file(df_train_sampled, 'fem', 'train-cicids2017', log1p=True)

    else:
        print(parser.print_help)
