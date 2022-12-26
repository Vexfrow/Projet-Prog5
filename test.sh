arrayFile=(./tests/*)



testHeader(){
    arm-none-eabi-readelf -h $1 > resultatAttendu
    ./main -h $1 > resultatObtenu

    if diff resultatObtenu resultatAttendu
    then    
        echo "[HEADER] OK pour $1"
    else
        echo "[HEADER] ECHEC pour $1"
        exit
    fi
}


#Revoir maniere de faire pour comparer ligne par ligne
testSectionHeader(){
    arm-none-eabi-readelf -S -W $1 | head -n -5 | tail -n +5 | sed -r 's/ //g' | sed -r 's/\t//g' > resultatAttendu   
    ./main -sh $1 | tail -n +3 | sed -r 's/SHT_//g' | sed -r 's/ARM_ATTR/ARM_ATTRIBUTES/g' | sed -r 's/ //g' | sed -r 's/\t//g'> resultatObtenu

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
    echo "[SECTION HEADER] OK pour $1"
    
}


#Revoir maniere de faire pour comparer ligne par ligne
testSymbolTable(){

    arm-none-eabi-readelf -s -W $1 | tail -n +4 | sed -r 's/ //g' | sed -r 's/\t//g' > resultatAttendu   
    ./main -st $1 | tail -n +2 | sed -r 's/ //g' | sed -r 's/\t//g'> resultatObtenu

    i=1
    while read -r ligneA;
    do
        ligneO=`cat resultatObtenu | head -n $i | tail -n -1`

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
    echo "[SYMBOL TABLE] OK pour $1"
 
}



make
for file in ${arrayFile[@]}
do
    testHeader $file
    testSectionHeader $file
    testSymbolTable $file
done


