
# Mani Amoozadeh: bash code for running the simulation on server


#######################################
# check to see if omnet++ is installed
#######################################

echo ""

OPPRUN=${HOME}"/omnetpp-4.2.2/bin/opp_run";

if [ ! -f $OPPRUN ]; then
   echo "OMNET++ can not be found!"
   exit -1
else
   echo "OMNET++ is installed."
fi

###########################
# add bin directory to path
###########################

echo "$PATH" | grep -q "omnetpp-4.2.2/bin"

if [ $? -eq 0 ]
then
   echo "omnet++ bin path is already in the path"
else   
   export PATH=$PATH:/home/maniam/omnetpp-4.2.2/bin
   echo "adding the bin folder into path"
fi

############################
# building the whole program
############################

echo "building the project..."

echo ""

make

######################################
# we first check to see if sumo exists
######################################

echo ""

SUMOPATH="/usr/bin/sumo"
 
if [ ! -f $SUMOPATH ];
then
   echo "sumo is not installed!"
   exit -1
else
   echo "sumo is installed."
fi

###############################
# then we run the python script
###############################

killall python
echo "launching python script in background ..."
echo ""

python sumo-launchd.py -vv -c sumo &

sleep 1

############################
# then we run the simulation
############################

echo ""
echo "run the simulation ..."
echo ""

x=429

while [ $x -le 639 ]
do

opp_runall -j2 opp_run -r $x..$(($x+1)) -u Cmdenv -c Sumo_Cmdenv -n .:../mixim/examples:../mixim/src:../mixim/tests -l application/application -l application/msg/msg -l ../mixim/tests/power/utils/powerTestUtils -l ../mixim/tests/testUtils/miximtestUtils -l ../mixim/src/base/miximbase -l ../mixim/src/modules/miximmodules ProjectConfig.ini

x=$(($x+2))
done


#for i in `seq 0 639`
#do
#opp_run -r $i -u Cmdenv -c Sumo_Cmdenv -n .:../mixim/examples:../mixim/src:../mixim/tests -l application/application -l application/msg/msg -l ../mixim/tests/power/utils/powerTestUtils -l ../mixim/tests/testUtils/miximtestUtils -l ../mixim/src/base/miximbase -l ../mixim/src/modules/miximmodules ProjectConfig.ini
#done

# wait for the above command to be completed
wait $!
echo ""
echo "simulation completed."


