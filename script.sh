for i in {"3.249","7.243","4.101","3.56","4.118","7.73","3.151","1.177","6.32","2.206","6.123"}; do echo "host $i"; sshpass -p "udesc" scp join.sh udesc@192.168.$i:~/; sshpass -p "udesc" ssh udesc@192.168.$i 'sh join.sh'; done