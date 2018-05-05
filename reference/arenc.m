function arcode=arenc(symbol,pr,seqin)
% Usage: arcode=arenc(symbol,pr,seqin)
% arithmetic encoding in decimal given a 
%   symbol probability table.
% output: 
%   arcode: a decimal number, the lower bound of the final interval.
% input:
%   symbol: the list of symbols, a row vector of single letters
%   pr:     the corresponding probabilty of each symbol
%   seqin:  the input sequence of symbols to be encoded.
% (C) 2002 by Yu Hen Hu
% created: 11/14/2002
%
high_range=[];
for k=1:length(pr),
   high_range=[high_range sum(pr(1:k))];
end
low_range=[0 high_range(1:length(pr)-1)];
sbidx=zeros(size(seqin));
for i=1:length(seqin),
   sbidx(i)=find(symbol==seqin(i));
end

low=0; high=1;
for i=1:length(seqin),
   range=high-low;
   high = low + range*high_range(sbidx(i));
   low = low + range*low_range(sbidx(i));
end
arcode=low;

