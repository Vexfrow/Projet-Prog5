arrayFile=(./tests/*)



testHeader(){
    arm-none-eabi-readelf -h $1 > resultatAttendue
    ./main -h $1 > resultatObtenue

    if diff resultatObtenue resultatAttendue
    then    
        echo "Test header réussie pour $1"
    else
        echo "Test header raté pour $1"
        exit
    fi
}



testSectionHeader(){
    arm-none-eabi-readelf -S $1 > resultatAttendue
    ./main -sh $1 > resultatObtenue

    if diff resultatObtenue resultatAttendue
    then    
        echo "Test header réussie pour $1"
    else
        echo "Test header raté pour $1"
        exit
    fi
}





make
for file in ${arrayFile[@]}
do
    testHeader $file
    testSectionHeader $file
done


