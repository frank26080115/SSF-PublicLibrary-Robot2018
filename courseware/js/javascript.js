var listOfVideos;
var currentVideo;

function makeAllHidables() {
	var rndId = 0;
	var listOfHidable = document.getElementsByTagName("fieldset");
	for (var i = 0; i < listOfHidable.length; i++)
	{
		var entry = listOfHidable[i];
		if (entry.id == undefined || entry.id === undefined || entry.id == null || entry.id === null || entry.id == "" || entry.id === "" || entry.id == '' || entry.id === '') {
			entry.id = "fld_" + rndId.toString();
			rndId += 1;
		}
		var hasClass = entry.classList.contains("hidable");
		if (hasClass == true)
		{
			var legend = entry.getElementsByTagName("legend")[0];
			legend.id = "lgd_" + entry.id;
			if (legend.getElementsByTagName("a").length <= 0) {
				legend.innerHTML += "<a> (click here to show)</a>";
			}
			var div = entry.getElementsByTagName("div")[0];
			div.style.display = "none";
			legend.onclick = function (event) {
				var legendId = this.id;
				var spanEle = this.getElementsByTagName("a")[0];
				var fldEle = document.getElementById(legendId.substring(4));
				var divEle = fldEle.getElementsByTagName("div")[0];
				if (divEle.style.display == "block" || divEle.style.display != "none") {
					divEle.style.display = "none";
					spanEle.innerHTML = " (click here to show)";
				}
				else {
					divEle.style.display = "block";
					spanEle.innerHTML = " (click here to hide)";
				}
			}
		}
	}
}

function makeAllVideos(poster) {
	var rndId = 0;
	var bodyDiv = document.getElementsByClassName("body_div")[0];
	listOfVideos = document.getElementsByTagName("video");
	for (var i = 0; i < listOfVideos.length; i++)
	{
		var entry = listOfVideos[i];
		if (entry.id == undefined || entry.id === undefined || entry.id == null || entry.id === null || entry.id == "" || entry.id === "" || entry.id == '' || entry.id === '') {
			entry.id = "vid_" + rndId.toString();
			rndId += 1;
		}
		var aspectRatio = 1.667;
		if (entry.videoWidth > 0 && entry.videoHeight > 0) {
			aspectRatio = entry.videoWidth / entry.videoHeight;
		}
		entry.poster = poster;
		entry.style.width = "100%";//bodyDiv.offsetWidth;
		//entry.style.height = entry.style.width / aspectRatio;
		entry.addEventListener("play", videoOnPlay, false);
	}

	/*
	//run the function every time the window is resized
	window.onresize = function (event) {
		keepAspectRatio(currentVideo, 16, 9, 1.0);
	}
	//*/
}

function makeTableOfContents(divName, titleTagName) {
	var ul = "";
	var listOfHeaders = document.getElementsByTagName(titleTagName);
	var cnt = 0;
	for (var i = 0; i < listOfHeaders.length; i++) {
		var entry = listOfHeaders[i];
		var givenId = entry.innerHTML.replace(/\W/g, '');
		entry.id = givenId;
		ul += '<li><a href="#' + givenId + '">' + entry.innerHTML + '</a></li>';
		cnt += 1;
	}
	if (cnt > 0)
	{
		var divEle = document.getElementById(divName);
		divEle.innerHTML = "<fieldset class='hidable'><legend>Table of Contents</legend><div><ul>" + ul + "</ul></div>";
	}
}

function videoOnPlay() {
	var id = this.id, i = 0;
	currentVideo = id;
	//keepAspectRatio(currentVideo, 16, 9, 1.0);
	for (var j = listOfVideos.length; i < j; i++) {
		if (listOfVideos[i].id !== id) {
			//keepAspectRatio(listOfVideos[i].id, 16, 9, 0.5);
			listOfVideos[i].pause();
		}
	}
}

function keepAspectRatio(id, width, height, scale) {
	var aspectRatioDiv = document.getElementById(id);
	aspectRatioDiv.style.width = window.innerWidth * scale;
	aspectRatioDiv.style.height = ((window.innerWidth / (width / height)) * scale) + "px";
}