trainrdkitnonan=readtable('train_rdkit.xlsx');
%testrdkitnonan=readtable('test_rdkit.xlsx');
%ncomp=height(trainrdkit);
nsize=size(trainrdkit)
ndesc=nsize(2) -2 %omit the first two columns, first column is for entry name and the second column is for experimental values
nrun=10
for d=1:ndesc
	cmdstr=['modify_fun_matlab.exe -i trainRegressionModel.m -o gsaModel.m -id ' num2str(d, '%d')]	
	system(cmdstr)
    train=trainrdkit;
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



