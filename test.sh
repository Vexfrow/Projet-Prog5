arrayFile=(./tests/*)
nbSections=0


testHeader(){
    arm-none-eabi-readelf -h $1 > resultatAttendu
    ./affichageElf -h $1 | head -n -1  > resultatObtenu

    if diff resultatObtenu resultatAttendu
    then    
        echo -e "\e[48;5;2m[HEADER] OK pour $1\e[0m"
    else
        echo -e "\e[48;5;1m[HEADER] ECHEC pour $1\e[0m"
        exit
    fi
}


testSectionHeader(){
    arm-none-eabi-readelf -S -W $1 | head -n -5 | tail -n +5 | sed -r 's/ //g' | sed -r 's/\t//g' > resultatAttendu   
    ./affichageElf -S $1 | tail -n +3 | sed -r 's/SHT_//g' | sed -r 's/ARM_ATTR/ARM_ATTRIBUTES/g' | sed -r 's/ //g' | sed -r 's/\t//g'> resultatObtenu

    i=1
    while read -r ligneA;
    do
        ligneO=`cat resultatObtenu | head -n $i | tail -n -1`

        if [ "$ligneA" != "$ligneO" ]
        then
            echo -e "\e[48;5;1m[SECTION HEADER] ECHEC pour $1 à la ligne $i \e[0m"
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
        echo -e "\e[48;5;2m[SECTION HEADER] OK pour $1\e[0m"
        nbVar=$(($i-2))
    else
        echo -e "\e[48;5;1m[SECTION HEADER] ECHEC pour $1 : Nombre de lignes différent\e[0m"
        exit
    fi
    
}


testSymbolTable(){

    arm-none-eabi-readelf -s -W $1 | tail -n +4 | sed -r 's/ //g' | sed -r 's/\t//g' > resultatAttendu   
    ./affichageElf -s $1 | tail -n +2 | sed -r 's/ //g' | sed -r 's/\t//g'> resultatObtenu

    i=1
    while read -r ligneA;
    do
        ligneO=`sed -n "$i"p resultatObtenu`

        if [ "$ligneA" != "$ligneO" ]
        then
            echo -e "\e[48;5;1m[SYMBOL TABLE] ECHEC pour $1 à la ligne $i \e[0m"
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
        echo -e "\e[48;5;2m[SYMBOL TABLE] OK pour $1\e[0m"
    else
        echo -e "\e[48;5;1m[SYMBOL TABLE] ECHEC pour $1 : Nombre de lignes différent\e[0m"
        exit
    fi
 
}


testSectionUnique(){

    for j in `seq 0 $nbVar`
    do
        arm-none-eabi-readelf -x $j $1 | cut -c 1-48 | tail -n +3 | sed -r 's/ //g' | sed -r 's/\t//g' > resultatAttendu   
        ./affichageElf -x $j $1 | cut -c 1-48 | tail -n +3 | sed -r 's/ //g' | sed -r 's/\t//g'> resultatObtenu

        if [[ `sed -n 1p resultatAttendu` == "NOTE:Thissectionhasrelocationsagainstit," ]]
        then 
            arm-none-eabi-readelf -x $j $1 | cut -c 1-48 | tail -n +4 | sed -r 's/ //g' | sed -r 's/\t//g' > resultatAttendu
        fi

        if diff resultatObtenu resultatAttendu
        then    
            echo -e "\e[48;5;2m[SECTION UNIQUE - $j ] OK pour $1\e[0m"
        else
            echo -e "\e[48;5;1m[SECTION UNIQUE - $j ] ECHEC pour $1\e[0m"
            echo "Ligne attendu :"
            echo `cat resultatAttendu`
            echo ""
            echo "Ligne Obtenu"
            echo `cat resultatObtenu`
            exit
        fi
    done
 
}

testRelocationTable(){

    arm-none-eabi-readelf -r -W $1 | tail -n +2 | sed -r 's/ //g' | sed -r 's/\t//g' > resultatAttendu   
    ./affichageElf -r $1 | head -n -1 | sed -r 's/ //g' | sed -r 's/\t//g'> resultatObtenu

    i=1
    while read -r ligneA;
    do
        ligneO=`sed -n "$i"p resultatObtenu`

        if [ "$ligneA" != "$ligneO" ]
        then
            echo -e "\e[48;5;1m[RELOCATION TABLE] ECHEC pour $1 à la ligne $i\e[0m "
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
        echo -e "\e[48;5;2m[RELOCATION TABLE] OK pour $1\e[0m"
    else
        echo -e "\e[48;5;1m[RELOCATION TABLE] ECHEC pour $1 : Nombre de lignes différent\e[0m"
        exit
    fi
 
}


#Chose à comparer : -Header -> Tout sauf offset de départ (pour le moment)
testFusion(){
    ./main ./tests/file1BigEndian.o ./tests/file2BigEndian.o 

    arm-none-eabi-readelf -h $1 > resultatAttendu
    ./affichageElf -h $1 | head -n -1  > resultatObtenu



 
}



make
for file in ${arrayFile[@]}
do
    echo "------------------------------------TEST SUR LE FICHIER "$file" - AFFICHAGE ELF ---------------------------------------"
    testHeader $file
    testSectionHeader $file
    testSectionUnique $file
    testSymbolTable $file
    testRelocationTable $file
    echo "------------------------------------GOOD FOR AFFICHAGE ELF------------------------------------------------------------------------"

    
done
#testFusion


