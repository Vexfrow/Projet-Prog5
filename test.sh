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



testSectionHeader(){
    sectionHeaderA=`arm-none-eabi-readelf -S $1 | head -n -5 | tail -n +5` #On récupère juste le tableau
    sectionHeaderB=`./main -sh $1 | tail -n +3`
    for ((i=0;i<=10;i++));
    do
        echo "${sectionHeaderA}" > resultatAttendu
        echo "${sectionHeaderB}" > resultatObtenu

        echo `cut -f ${i+1} resultatAttendu` > resultatAttendu
        echo `cut -f ${i+1} resultatObtenu` > resultatObtenu

        cat resultatAttendu

        #if diff resultatObtenu resultatAttendu
        #then    
         #   echo "Test header section réussie pour $1"
        #else
          #  echo "Test header raté pour $1"
         #   exit
        #fi
    done
}






make
for file in ${arrayFile[@]}
do
    testHeader $file
    testSectionHeader $file
done


