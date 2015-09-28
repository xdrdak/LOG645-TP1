#!/bin/bash
echo -n "Lab 1 - Run file incroyable pour lancer l'application parallèle"
echo ""
echo -n "Entrez le choix du programe (1 ou 2): "
read choice
echo -n "Entrez la valeur initiale 'p' (Doit être >=1) : "
read p
echo -n "Entrez le nombre d'itération 'n' (Doit être >=1) : "
read n
if ([ $choice -eq 1 ] || [ $choice -eq 2 ]) && [ $p -gt 0 ] && [ $n -gt 0 ]; then
  mpirun --hostfile hostfile -np 17 tp1 $choice $p $n
else
  echo "Attention : vous avez des erreurs dans vos paramètres"
fi
