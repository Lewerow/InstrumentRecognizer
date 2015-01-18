@echo off
mkdir summaries
grep -nre "Mean\|Training time" data | grep "Summary" > summaries/summaries.txt

cd summaries

for %%d in (iris, glass, wine) do (
    @echo on
	echo Generating summary for %%d
	@echo off
	
	mkdir %%d
	grep %%d summaries.txt > %%d/summary.txt
	
	cd %%d
	call ../../report_single.bat
	cd ..
)
cd ..