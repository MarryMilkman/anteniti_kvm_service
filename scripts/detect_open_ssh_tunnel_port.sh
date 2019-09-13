ss -a -p -t |grep 127.0.0.1 | awk '{print $4}' | awk -F: '{print $2}'
