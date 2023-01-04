arrayFile=(./tests/*)
nbSections=0


testHeader(){
    arm-none-eabi-readelf -h $1 > resultatAttendu
    ./main -h $1 | head -n -1  > resultatObtenu

    if diff resultatObtenu resultatAttendu
    then    
        echo "[HEADER] OK pour $1"
    else
        echo "[HEADER] ECHEC pour $1"
        exit
    fi
}


testSectionHeader(){
    arm-none-eabi-readelf -S -W $1 | head -n -5 | tail -n +5 | sed -r 's/ //g' | sed -r 's/\t//g' > resultatAttendu   
    ./main -S $1 | tail -n +3 | sed -r 's/SHT_//g' | sed -r 's/ARM_ATTR/ARM_ATTRIBUTES/g' | sed -r 's/ //g' | sed -r 's/\t//g'> resultatObtenu

    i=1
    while read -r ligneA;
    do
        ligneO=`cat resultatObtenu | head -n $i | tail -n -1`

        if [ "$ligneA" != "$ligneO" ]
        then
            echo "[SECTION HEADER] ECHEC pour $1 à la ligne $i "
            echo ""
            echo "Ligne attendu :"
            echo "$ligneA"
            echo ""
            echo "Ligne Obtenu"
            echo "$ligneO"
            exit
        fi

        ((i+=1))
    done < resultatAttendu

    if [[ `sed -n "$i"p resultatObtenu | wc -w` -eq 0 ]]
    then 
        echo "[SECTION HEADER] OK pour $1"
        nbVar=$(($i-2))
    else
        echo "[SECTION HEADER] ECHEC pour $1 : Nombre de lignes différent"
        exit
    fi
    
}


testSymbolTable(){

    arm-none-eabi-readelf -s -W $1 | tail -n +4 | sed -r 's/ //g' | sed -r 's/\t//g' > resultatAttendu   
    ./main -s $1 | tail -n +2 | sed -r 's/ //g' | sed -r 's/\t//g'> resultatObtenu

    i=1
    while read -r ligneA;
    do
        ligneO=`sed -n "$i"p resultatObtenu`

        if [ "$ligneA" != "$ligneO" ]
        then
            echo "[SYMBOL TABLE] ECHEC pour $1 à la ligne $i "
            echo ""
            echo "Ligne attendu :"
            echo "$ligneA"
            echo ""
            echo "Ligne Obtenu"
            echo "$ligneO"
            exit
        fi

        ((i+=1))
    done < resultatAttendu


    if [[ `sed -n "$i"p resultatObtenu | wc -w` -eq 0 ]]
    then 
        echo "[SYMBOL TABLE] OK pour $1"
    else
        echo "[SYMBOL TABLE] ECHEC pour $1 : Nombre de lignes différent"
        exit
    fi
 
}


testSectionUnique(){

    for j in `seq 0 $nbVar`
    do
        arm-none-eabi-readelf -x $j $1 | cut -c 1-48 | tail -n +3 | sed -r 's/ //g' | sed -r 's/\t//g' > resultatAttendu   
        ./main -x $j $1 | cut -c 1-48 | tail -n +3 | sed -r 's/ //g' | sed -r 's/\t//g'> resultatObtenu

        if [[ `sed -n 1p resultatAttendu` == "NOTE:Thissectionhasrelocationsagainstit," ]]
        then 
            arm-none-eabi-readelf -x $j $1 | cut -c 1-48 | tail -n +4 | sed -r 's/ //g' | sed -r 's/\t//g' > resultatAttendu
        fi

        if diff resultatObtenu resultatAttendu
        then    
            echo "[SECTION UNIQUE - $j ] OK pour $1"
        else
            echo "[SECTION UNIQUE - $j ] ECHEC pour $1"
            exit
        fi
    done
 
}



make
for file in ${arrayFile[@]}
do
    testHeader $file
    testSectionHeader $file
    testSectionUnique $file
    testSymbolTable $file
    echo "--------------------------------------------------------------------------------------"
    
done


