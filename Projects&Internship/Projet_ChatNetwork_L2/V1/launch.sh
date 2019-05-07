#!/bin/bash
python3 server.py &
sleep 1 &
xterm -title "Alice" -hold -e python3 client.py &
xterm -title "Bob" -hold -e python3 client.py &
xterm -title "Charles" -hold -e python3 client.py #&
#xterm -title "Daphnée" -hold -e python3 client.py &
