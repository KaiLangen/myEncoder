% arcodemo.m: demonstration of arithmetic coding
% (C) 2002 by Yu Hen Hu
% created: 11/14/2002
% calls arenc.m, ardec.m

clear all;
close all;
clc;
format long;
symbol=['_abe'];
pr=[1 2 3 2]/8; % div by sum of prob.
seqin=['be_a_bee'];
codeword=arenc(symbol,pr,seqin)

seqout=ardec(symbol,pr,codeword,8) % change the #

