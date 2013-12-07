window.onload = function() {
	var upload_ready = false;
	var binary_data = [];
	var color = {
		red: 0,
		blue: 0,
		green: 0
	};
	var refresh = 18;
	var upload_time;
	/* Each refresh is a flash
	   Each flash is 3 bytes
	   A second is 1000 m seconds
	   bandwidth is flashe_size * (1000/refresh)
	  */
	var bandwidth = (1000/refresh)*3;
	console.log("bandwidth = "+bandwidth);

	var fileInput = document.getElementById('fileInput');
	var fileDisplayArea = document.getElementById('fileDisplayArea');
	var startUpload = document.getElementById('startUpload');
	var statusBar = document.getElementById('status-bar');
	var statusText = document.getElementById('status-text');

	fileInput.addEventListener('change', function(e) {
		var file = fileInput.files[0];
		var reader = new FileReader();

		reader.onload = function(e) {
			var status_text = document.getElementById('status-text');
			upload_ready = true;
			var data = reader.result;
			binary_data = [];
			for(var i = 0; i < reader.result.length; i++) {
				binary_data.push(data.charCodeAt(i));
			}
			fileDisplayArea.innerText = reader.result;
			upload_time = (binary_data.length/bandwidth).toFixed(2);
			statusText.innerText = "Hex File Uploaded, Will take "+upload_time+" seconds to complete.";
		}
		reader.readAsBinaryString(file);
	});

	startUpload.addEventListener('click', function(e) {
		var counter = 0;
		console.log("starting upload");
		console.time("upload time");
		var callback = setInterval(function() {
			color.red = binary_data[counter++];
			color.blue = binary_data[counter++];
			color.green = binary_data[counter++];

			fileDisplayArea.style.backgroundColor = "rgb("+color.red+","+color.green+","+color.blue+")";

			var percent = counter/binary_data.length;
			if(percent > 1) { percent = 1; }
			var timeLeft = upload_time * (1-percent);
			percent = (percent*100).toFixed(2);

			statusBar.style.width = percent+"%";
			statusText.innerText = "Upload "+percent+"% Complete ( "+timeLeft.toFixed(2)+" seconds left )";

			if(counter >= binary_data.length) {
				console.timeEnd("upload time");
				statusBar.style.width = percent+"%";
				statusText.innerText = "Upload 100% Complete ( 0.00 seconds left )";
				fileDisplayArea
				clearInterval(callback);
			}
		}, refresh);
	});
}
