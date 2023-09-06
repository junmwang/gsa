# gsa
Perform global sensitivity analysis
One needs to save the model function of a trained model which is trained using "Regression Learner". The function name is trainRegressionModel.
The C-code modifies the function "trainRegressionModel" by omitting a specific descriptor. Then one do "nrun" training to investigate the indispensability of the variable. 
