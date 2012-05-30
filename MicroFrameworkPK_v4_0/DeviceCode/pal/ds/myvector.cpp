
#include "address_table.h"


#define DATASTORE_ASSERT(x, y)
#define DATASTORE_DBG(x, y)

/****************************************************************************
*
*  Function Name :
*
******************************************************************************/
/*!
*  \brief
*
*  \param
*  \param
*  \return
*
******************************************************************************/
int myVector::insert ( int position, const DATASTORE_ADDR_TBL_ENTRY& x )
{

    if(vectSize < position || position < 0){
        DATASTORE_DBG( false,
                       "Inserting beyond vectSize is not supported" );
        return -1;
    }

    for(int index = vectSize; index > position; index--){
        intTable[index] = intTable[index - 1];
    }
    intTable[position] = x;
    vectSize++;
    return position;

}


/****************************************************************************
*
*  Function Name :
*
******************************************************************************/
/*!
*  \brief
*
*  \param
*  \param
*  \return
*
******************************************************************************/
void myVector::clear()
{

    memset(intTable, 0, sizeof(intTable));  /* Should I be doing this?s */
    vectSize = 0;
    return;

}



/****************************************************************************
*
*  Function Name :
*
******************************************************************************/
/*!
*  \brief
*
*  \param
*  \param
*  \return
*
******************************************************************************/
int myVector::erase(int position)
{

    assert(position > 0 && position < vectSize);
    if(position < 0 || position >= vectSize ){
        DATASTORE_ASSERT(false, "Invalid location");
        return -1;
    }

    for(int index = position; index < vectSize-1; index++){
        intTable[index] = intTable[index+1];
    }
    memset(&intTable[vectSize-1], 0, sizeof(DATASTORE_ADDR_TBL_ENTRY));
    vectSize--;
    return position;


}

/****************************************************************************
*
*  Function Name :
*
******************************************************************************/
/*!
*  \brief
*
*  \param
*  \param
*  \return
*
******************************************************************************/
DATASTORE_ADDR_TBL_ENTRY& myVector::operator[] (const int index)
{

    ASSERT(index < vectSize);
    return intTable[index];

}


