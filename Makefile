.PHONY: benchmarks
benchmarks:
	docker build -t jajson-benchmarking .
	docker run -it -d --name jajson-container jajson-benchmarking 