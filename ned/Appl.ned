package ned;

simple Appl like IAppl
{
    parameters:
        @class(Appl);  
    
    gates:
    	input lowerLayerIn; // from network layer
        output lowerLayerOut; // to network layer
        
        input lowerControlIn; // control from network layer
        output lowerControlOut; // control to network layer
}