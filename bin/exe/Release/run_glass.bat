for %%u in (EqualSizes, EqualFrequencies) do (
	for /l %%x in (2, 1, 10) do (
		for /l %%y in (3, 1, 15) do (
			call single_glass.bat %%1 %%u_%%y %%x
		)
	)
)