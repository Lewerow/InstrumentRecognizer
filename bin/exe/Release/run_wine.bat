for %%u in (EqualSizes, EqualFrequencies) do (
	for /l %%x in (2, 1, 10) do ( REM FOLDS
		for /l %%y in (3, 1, 15) do ( REM DISCRETIZATION CLASSES
			call single_wine.bat %%1 %%u_%%y %%x
		)
	)
)