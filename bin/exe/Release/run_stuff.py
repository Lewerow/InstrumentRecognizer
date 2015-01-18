from subprocess import call

data = {"glass": {"name": "glass", "class_col" : 9},
  "wine" : {"name": "wine", "class_col" : 0},
  "iris" : {"name": "iris", "class_col" : 4},
#  "seeds" : {"name" : "seeds", "class_col": 7},
#  "diabetes" : {"name" : "diabetes", "class_col": 8}
}

algos = ["NaiveBayes_7_EqualSizes", "ILA_7_EqualFrequencies", "KNN_5_Euclid_Weighted"]
n = 1
for _, dataset in data.iteritems():
	file = "\\".join(["data", dataset["name"], dataset["name"] + ".csv"])
	descs = "\\".join(["data", dataset["name"], "descriptions"])
	reports = "\\".join(["data", dataset["name"]])
	  
	cv = 10
	for base in algos:
	    for k in [3, 5, 7, 9, 11, 13, 15, 17, 19]:
		    for alg in ["Bagging", "Boosting"]:
				classifier = "_".join([alg, str(k), base])
				report = reports + "\\reports_cv" + str(cv) + "_" + classifier
				call(["mkdir", report])
				print(str(n) + ".) Currently running: " + dataset["name"] + " Classifier: " + classifier + " CV:" + str(cv))
				call(["Recognizer.exe", "--step=all", "--data_file=" + file, "--class_name_column="+str(dataset["class_col"]), "--description_dir="+descs, "--reports_dir="+report, "--fold_count="+str(cv), "--classifier_type=" + classifier])
				n = n + 1