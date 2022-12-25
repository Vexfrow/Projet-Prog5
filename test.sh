arrayFile=(./tests/*)



testHeader(){
    arm-none-eabi-readelf -h $1 > resultatAttendu
    ./main -h $1 > resultatObtenu

    if diff resultatObtenu resultatAttendu
    then    
        echo "Test header réussie pour $1"
    else
        echo "Test header raté pour $1"
        exit
    fi
}


#Revoir maniere de faire pour comparer ligne par ligne
testSectionHeader(){
    echo `arm-none-eabi-readelf -S $1 | head -n -5 | tail -n +5` > resultatAttendu    #On récupère juste le tableau
    echo `./main -sh $1 | tail -n +3 | sed -r 's/SHT_NULL/NULL/g' | sed -r 's/SHT_NULL/NULL/g' | sed -r 's/SHT_PROGBITS/PROGBITS/g' | sed -r 's/SHT_NOBITS/NOBITS/g' | sed -r 's/SHT_ARM_ATTR/ARM_ATTRIBUTES/g' | sed -r 's/SHT_SYMTAB/SYMTAB/g' | sed -r 's/SHT_STRTAB/STRTAB/g' | sed -r 's/SHT_REL/REL/g'`> resultatObtenu
    echo `grep -o [a-zA-Z0-9.] resultatAttendu` > resultatAttendu
    echo `grep -o [a-zA-Z0-9.] resultatObtenu` > resultatObtenu
  
    if diff resultatObtenu resultatAttendu
    then    
        echo "Test header section réussie pour $1"
    else
        echo "Test header section raté pour $1"
        exit
    fi
 
}






make
for file in ${arrayFile[@]}
do
    #testHeader $file
    testSectionHeader $file
done


