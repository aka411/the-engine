# ECS Subsystem





## Iterator design


### Methods


iterator.peekNext(componentType)
iterator.getNext(componentType)

iterator.offsetStart(componentType,offset)


#### main iterator and sub iterator

sub iterator for each component , main iterator contains all the chunk for all components of required filter 
then we can request sub iterator from main iterator for each compoenent



## Another design of iterator 

look into chunked iteration where we cast a chunk as the compoenent type and use [];

maybe the sub iterator can use this under its implementation.