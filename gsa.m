trainrdkitnonan=readtable('train_rdkit_no_nan.xlsx');
testrdkitnonan=readtable('test_rdkit_no_nan.xlsx');
%ncomp=height(trainrdkitnonan);
nsize=size(trainrdkitnonan)
ndesc=nsize(2) -2 %omit the first two columns
nrun=5
for d=1:ndesc
	cmdstr=['modify_fun_matlab.exe -i trainRegressionModel.m -o gsaModel.m -id ' num2str(d, '%d')]	
	system(cmdstr)
    train=trainrdkitnonan;
    train(:,d+2)=[]
    vrmse(1) = d
	for num = 1:nrun
 		tic
		[gsa_model, validationRMSE] = gsaModel(train)
		toc
		vrmse(num+1)=validationRMSE;
	end
    writematrix(vrmse, 'gsa.txt', 'WriteMode','append'); 
end



