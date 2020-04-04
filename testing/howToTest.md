##### Create a thread per request version is inside the threadPerRequest directory
##### Pre-create a set of thread version is inside the preCreateRequest directory

To run any version server, cd into its folder where docker-compose.yml is in (threadPerRequest or preCreateRequest), run:

```
sudo docker-compose up
```
to run the server side. And now we just hardcode 32 buckets inside docker-compose.yml, to change it, just change the "command"
line inside the docker-compose.yml, the format is 

```
./server <bucket number>
```

To run the client for any version, cd into the src directory inside threadPerRequest or preCreateRequest and run:

```
./client <bucket number>
```
