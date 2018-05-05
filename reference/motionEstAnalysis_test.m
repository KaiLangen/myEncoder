% This script uses Motion Estimation algorithms
% written by Aroh Barjatya, modified by Khan Wahid, feb 2010.

clc;
close all
clear all

mbSize = 16; %play with diff. block size
p = 7; % change p and see how the # of computations change

%loading images / frames from files
% imgI = ref (say frame 1, previous, reference)
% imgP = target (say frame 2, next after I, current)

% imgI = double(imread('Martin_011.jpg'));
% imgP = double(imread('Martin_012.jpg'));

% imgI = double(imread('cir_mov1.tif'));
% imgP = double(imread('cir_mov2.tif'));

% imgI = double(imread('nasa1.tif'));
% imgP = double(imread('nasa2.tif'));

%loading frames from video (carphone.qcif, foreman.qcif)
fid = fopen('carphone.qcif','r');
imgI = double(readYFrame(fid,10,'qcif'));
imgP = double(readYFrame(fid,15,'qcif')); 
% For carphone video, choose frame#10 & #20; in #20, see the lamppost
% try 20, 30 => the lamppost disappers in the MC frame => error => needs bi-direticonal prediction
% try 18, 19, lamppost first appears in frame 19
% now try #19, 20 => both excellent results

%read avi video, different syntax to read avi file
% framedata1=aviread('news.avi',11); % see 20 and 31, left leg cannot be predicted irrespective of MC
% framedata2=aviread('news.avi',12); %1st scene change in #91, see 90, 91, 92
% imgI=double(frame2im(framedata1));
% imgP=double(frame2im(framedata2));

% row and column
[row, col] = size(imgI);
    
% Three Step Search
[motionVect,computations ] = motionEstTSS(imgP,imgI,mbSize,p);
imgComp = motionComp(imgI, motionVect, mbSize);
psnr = imgPSNR(imgP, imgComp, 255)
computations = computations
    
dx=motionVect(1,:);
dxr=reshape(dx,row/mbSize,col/mbSize); % 1 2 3; 4 5 6; 7 8 9...
dy=motionVect(2,:);
dyr=reshape(dy,row/mbSize,col/mbSize);
    
err = imgP-imgI; % frame2 - frame1
err_mc = imgP-imgComp;% frame2 - predicted frame1
ent_ori = frame_entropy(err);
ent_mc = frame_entropy(err_mc);

figure
subplot(2,3,2); imshow(uint8(imgP)), title 'Frame2 (current)';
subplot(2,3,1); imshow(uint8(imgI)), title 'Frame1 (previous/reference)';
subplot(2,3,3); imshow(uint8(imgComp)), title (['Motion compensated Frame1 ', '[psnr: ', int2str(psnr), ']']);
subplot(2,3,4); quiver(flipud(dxr'),flipud(dyr')), title 'Motion Vectors';
subplot(2,3,5); imshow(uint8(err)), title (['Residue Image (Non-MC) ', '[H: ', num2str(ent_ori), ']']);
subplot(2,3,6); imshow(uint8(err_mc)), title (['Residue Image (MC) ', '[H: ', num2str(ent_mc), ']']);

% Note that, a non-MC coder will send either MV (if OF used) or residue
% image (if DPCM used) => the decoder will reconstruct frames using them

% An MC coder will send two: MV and MC-ed residue. prev + MV will produce a
% frame that looks like previous reconstructed frame (like plot 233). But
% there are error, so, need to send the MC-ed residue will then be added in
% the decoder to create the actual frame2.
