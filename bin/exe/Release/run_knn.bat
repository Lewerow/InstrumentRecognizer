@echo off


set DATA_FILE=%1
set CLASS_NAME_COLUMN=%2
set DESCRIPTION_DIR=%3
set REPORT_ROOT=%4

for %%u in (Euclid Manhattan Minkowski3) do (
	for /l %%x in (Majority Weighted DualWeighted) do (
		for /l %%k in (1 3 5 7 9 11) do (
			for /l %%c in (2 3 5 7 10) do (
			    @echo off
				set REPORT_DIR=%REPORT_ROOT%\reports_cv%%c
                mkdir %REPORT_DIR%
    			@echo on
				Recognizer.exe --step=all --data_file=%DATA_FILE% --class_name_column=%CLASS_NAME_COLUMN% --description_dir=%DESCRIPTION_DIR% --reports_dir=%REPORT_DIR% --fold_count=%%c --classifier_type=KNN_%%u_%%x_K%%k
			)
		)
	)
)