# cc-cw2
## Section 1
Implement 1 sec timer based sceduling which uses a list of processes and removes items from the list when they are complete i.e. they return a value

## Section 2
### Kill
If killing current process, reset ctx and run scheduler in order to imediatly switch to the next process. If not current proccess then reset the processes ctx but continue execution of current process (rather then running the scheduler again).
Also set all orphaned processes ppid to 0

TODO:
- What happens when program finsihes and is started again with fork?
- Warning when killing own proccess?
- What happens when try to start a proccess if there is not enough room
