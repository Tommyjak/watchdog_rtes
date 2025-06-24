#/bin/bash

echo "plotting..."

cp "$HOME/minicom.log" "$HOME/Documents/universita/RTES/watchdog_rtes/tools/minicom.log"

cp "$HOME/minicom_seq.log" "$HOME/Documents/universita/RTES/watchdog_rtes/tools/minicom_seq.log"

python3 tools/log_parser.py

echo "done!"