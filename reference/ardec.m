function symseq=ardec(symbol,pr,codeword,symlen)
% Usage: symseq=ardec(symbol,pr,codeword,symlen)
% arithmetic decoding in decimal given a 
%   symbol probability table.
% output: 
%   symseq:  the input sequence of symbols to be encoded.
% input:
%   symbol: the list of symbols, a row vector of single letters
%   pr:     the corresponding probabilty of each symbol
%   codeword: a decimal number, the lower bound of the final interval.
%   symlen: length of the decoded symbol sequence.
% (C) 2002 by Yu Hen Hu
% created: 11/14/2002
%
format long
high_range=[];
for k=1:length(pr),
   high_range=[high_range sum(pr(1:k))];
end
low_range=[0 high_range(1:length(pr)-1)];
prmin=min(pr); 
symseq=[];
for i=1:symlen, 
   idx=max(find(low_range<=codeword));
   codeword=codeword-low_range(idx);
   % due to numerical error, sometimes the encoded number 
   % will be slightly smaller than the current lower bound.
   % If this happens, a correction is required.
   if abs(codeword-pr(idx))< 0.01*prmin, 
      idx=idx+1; 
      codeword=0;
   end
   symseq=[symseq symbol(idx)],
   codeword=codeword/pr(idx),
   if abs(codeword)<0.01*prmin,
      i=symlen+1;  % break the for loop immediately
   end
end

