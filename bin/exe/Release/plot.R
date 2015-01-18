setwd("D:/C++/InstrumentRecognizer/bin/exe/Release")

draw_cv_series <- function(tab, metric, vote){

	metric_vote=tab[tab[1] == metric & tab[2] == vote,]
	metric_vote=metric_vote[order(metric_vote[,3]),]

	metric_vote_cv2 = metric_vote[metric_vote[4] == 2,]
	metric_vote_cv3 = metric_vote[metric_vote[4] == 3,]
	metric_vote_cv5 = metric_vote[metric_vote[4] == 5,]
	metric_vote_cv7 = metric_vote[metric_vote[4] == 7,]
	metric_vote_cv10 = metric_vote[metric_vote[4] == 10,]

	miny = min(metric_vote[,8])*0.9
	maxy = max(metric_vote[,8])*1.1

	minx = min(metric_vote[,3])*0.9
	maxx = max(metric_vote[,3])*1.1

	#FScore
	jpeg(paste(metric, vote, "fscore_vs_neighbors_folds_as_series.jpg"))
	plot(1, 1, xlim=c(minx, maxx), ylim=c(miny, maxy), xlab="Rozmiar sasiedztwa", ylab="F-Score")
	title(main=paste("F-Score w zaleznosci od rozmiaru sasiedztwa dla\n miary odleglosci:", metric, " i glosowania: ", vote))

	points(metric_vote_cv2[,3], metric_vote_cv2[,8], col="blue", pch=19)
	points(metric_vote_cv3[,3], metric_vote_cv3[,8], col="red", pch=20)
	points(metric_vote_cv5[,3], metric_vote_cv5[,8], col="green", pch=21)
	points(metric_vote_cv7[,3], metric_vote_cv7[,8], col="black", pch=22)
	points(metric_vote_cv10[,3], metric_vote_cv10[,8], col="orange",pch=23)

	legend("bottomright", legend=c("2 foldy", "3 foldy", "5 foldow", "7 foldow", "10 foldow"), 
		pch=c(19,20,21,22,23), col=c("blue", "red", "green", "black", "orange"))
	dev.off()
} 

draw_metric_series <- function(tab, vote){

	metric_vote=tab[tab[2] == vote & tab[4] == 10,]
	metric_vote=metric_vote[order(metric_vote[,3]),]

	vote_euclid = metric_vote[metric_vote[1] == "Euclid",]
	vote_manhattan = metric_vote[metric_vote[1] == "Manhattan",]
	vote_minkowski3 = metric_vote[metric_vote[1] == "Minkowski3",]

	miny = min(metric_vote[,8])*0.9
	maxy = max(metric_vote[,8])*1.1

	minx = min(metric_vote[,3])*0.9
	maxx = max(metric_vote[,3])*1.1

	#FScore
	jpeg(paste(metric, vote, "fscore_vs_neighbors_metrics_as_series.jpg"))
	plot(1, 0, xlim=c(minx, maxx), ylim=c(miny, maxy), xlab="Rozmiar sasiedztwa", ylab="F-Score")
	title(main=paste("F-Score w zaleznosci od rozmiaru sasiedztwa dla\n glosowania:", vote, " i 10 foldow walidacji krzyzowej"))

	points(vote_euclid[,3], vote_euclid[,8], col="blue", pch=19)
	points(vote_manhattan[,3], vote_manhattan[,8], col="red", pch=20)
	points(vote_minkowski3[,3], vote_minkowski3[,8], col="green", pch=21)

	legend("bottomright", legend=c("Euklidesowa", "Manhattan", "Minkowskiego"), 
		pch=c(19,20,21), col=c("blue", "red", "green"))
	dev.off()
} 

draw_vote_series <- function(tab, metric){

	metric_vote=tab[tab[1] == metric & tab[4] == 10,]
	metric_vote=metric_vote[order(metric_vote[,3]),]

	metric_majority = metric_vote[metric_vote[2] == "Majority",]
	metric_weighted = metric_vote[metric_vote[2] == "Weighted",]
	metric_dual = metric_vote[metric_vote[2] == "DualWeighted",]

	miny = min(metric_vote[,8])*0.9
	maxy = max(metric_vote[,8])*1.1

	minx = min(metric_vote[,3])*0.9
	maxx = max(metric_vote[,3])*1.1

	#FScore
	jpeg(paste(metric, vote, "fscore_vs_neighbors_metrics_as_series.jpg"))
	plot(1, 0, xlim=c(minx, maxx), ylim=c(miny, maxy), xlab="Rozmiar sasiedztwa", ylab="F-Score")
	title(main=paste("F-Score w zaleznosci od rozmiaru sasiedztwa dla\n miary:", metric, " i 10 foldow walidacji krzyzowej"))

	points(metric_majority[,3], metric_majority[,8], col="blue", pch=19)
	points(metric_weighted[,3], metric_weighted[,8], col="red", pch=20)
	points(metric_dual[,3], metric_dual[,8], col="green", pch=21)

	legend("bottomright", legend=c("Wiekszosciowa", "Wazona", "Podwojnie wazona"), 
		pch=c(19,20,21), col=c("blue", "red", "green"))
	dev.off()
} 

draw_xxx_vs_k <- function(tab, k, what, type) {
	bagging_ila = tab[tab[1] == "Bagging" & tab[3] == "ILA",]
	bagging_bayes = tab[tab[1] == "Bagging" & tab[3] == "NaiveBayes",]
	bagging_knn = tab[tab[1] == "Bagging" & tab[3] == "KNN",]
	boosting_ila = tab[tab[1] == "Boosting" & tab[3] == "ILA",]
	boosting_bayes = tab[tab[1] == "Boosting" & tab[3] == "NaiveBayes",]
	boosting_knn = tab[tab[1] == "Boosting" & tab[3] == "KNN",]
    
	miny = min(tab[,k])*0.8
	maxy = max(tab[,k])*1.1

	minx = min(tab[,2])*0.8
	maxx = max(tab[,2])*1.1

	#FScore
	jpeg(paste(type, paste(what, "_vs_k.jpg")))
	plot(1, 0, xlim=c(minx, maxx), ylim=c(miny, maxy), xlab="Liczba klasyfikatorow", ylab=what)
	title(main=paste(what, " w zaleznosci od liczby klasyfikatorow w zespole"))

	points(bagging_ila[,2], bagging_ila[,k], col="blue", pch=19)
	points(bagging_bayes[,2], bagging_bayes[,k], col="red", pch=20)
	points(bagging_knn[,2], bagging_knn[,k], col="black", pch=21)
	points(boosting_ila[,2], boosting_ila[,k], col="green", pch=22)
	points(boosting_bayes[,2], boosting_bayes[,k], col="orange", pch=23)
	points(boosting_knn[,2], boosting_knn[,k], col="purple", pch=24)

	legend("bottomright", legend=c("Bagging ILA", "Bagging Bayes", "Bagging KNN", "Boosting ILA", "Boosting Bayes", "Boosting KNN"), 
		pch=c(19,20,21,22, 23, 24), col=c("blue", "red", "black", "green", "orange", "purple"))
	dev.off()
}

draw_fmeasure_vs_k <- function(tab, type) {
    draw_xxx_vs_k(tab, 7, "F-Score", type)
}

draw_recall_vs_k <- function(tab, type) {
    draw_xxx_vs_k(tab, 5, "Recall", type)
}

draw_accurracy_vs_k <- function(tab, type) {
    draw_xxx_vs_k(tab, 4, "Accurracy", type)
}

draw_precision_vs_k <- function(tab, type) {
    draw_xxx_vs_k(tab, 6, "Precision", type)
}

setwd("summaries/glass")
glass=read.csv("tab.txt", TRUE, ",")
draw_fmeasure_vs_k(glass, "glass")
draw_precision_vs_k(glass, "glass")
draw_recall_vs_k(glass, "glass")
draw_accurracy_vs_k(glass, "glass")

setwd("../iris")
iris=read.csv("tab.txt", TRUE, ",")
draw_fmeasure_vs_k(iris, "iris")
draw_precision_vs_k(iris, "iris")
draw_recall_vs_k(iris, "iris")
draw_accurracy_vs_k(iris, "iris")

setwd("../wine")
wine=read.csv("tab.txt", TRUE, ",")
draw_fmeasure_vs_k(wine, "wine")
draw_precision_vs_k(wine, "wine")
draw_recall_vs_k(wine, "wine")
draw_accurracy_vs_k(wine, "wine")