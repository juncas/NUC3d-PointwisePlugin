/****************************************************************************
 *
 * NUC3d implementation of runtimeWrite(), runtimeCreate(), and
 * runtimeDestroy()
 *
 * Proprietary software product of Pointwise, Inc.
 * Copyright (c) 1995-2014 Pointwise, Inc.
 * All rights reserved.
 *
 ***************************************************************************/

#include "apiCAEP.h"
#include "apiCAEPUtils.h"
#include "apiGridModel.h"
#include "apiPWP.h"
#include "runtimeWrite.h"
#include "pwpPlatform.h"

 const int NUC3dfaceId[6] = {
    4,  /* PWGM_FACE_KMIN */
    5,  /* PWGM_FACE_KMAX */
    0,  /* PWGM_FACE_IMIN */
    1,  /* PWGM_FACE_IMAX */
    2,  /* PWGM_FACE_JMIN */
    3   /* PWGM_FACE_JMAX */
 };

typedef struct BC_TOPO_STR
{
    int type;
    int id;
}bc_topo;

static PWP_BOOL
writeTopo(CAEP_RTITEM *pRti, PWGM_HBLOCK block, PWP_UINT32 blkidx)
{
    PWP_BOOL ret = PWP_FALSE;

    PWP_UINT32 bndry_num=PwBlkBoundaryCount(block);
    PWGM_HBNDRY boundary;
    PWGM_BNDRYDATA bndryData;
    PWGM_CONDDATA condData;

    PWP_UINT32 cnx_num=PwBlkConnectionCount(block);
    PWGM_HCNXN connection;
    PWGM_CNXNDATA CnxnData;

    bc_topo myBCTopo[6];

    char filename[25];

    if (pRti && PWGM_HBLOCK_ISVALID(block)) 
    {
        sprintf(filename,"bc_topo/BC_Topo_%d.in\0",blkidx);      

        pRti->fp = pwpFileOpen(filename,(sysFILEMODE)(pwpWrite | pwpAscii)); 

        for(int idxBC=0;idxBC<bndry_num;idxBC++)
        {
            boundary=PwBlkEnumBoundaries(block,idxBC);
            PwBndryCondition(boundary,&condData);
            PwBoundary(boundary, &bndryData);

            myBCTopo[NUC3dfaceId[bndryData.face]].type=-1;
            myBCTopo[NUC3dfaceId[bndryData.face]].id=condData.tid;
        }

        for(int idxBC=0;idxBC<cnx_num;idxBC++)
        {
            connection=PwBlkEnumConnections(block,idxBC);
            PwConnection(connection,&CnxnData);

            myBCTopo[NUC3dfaceId[CnxnData.face1]].type=PWGM_HBLOCK_ID(CnxnData.block2);
            myBCTopo[NUC3dfaceId[CnxnData.face1]].id=NUC3dfaceId[CnxnData.face2];
            printf("type = %d, id = %d \n", 
                myBCTopo[NUC3dfaceId[CnxnData.face1]].type,
                myBCTopo[NUC3dfaceId[CnxnData.face1]].id);
        }

        for(int iface=0;iface<6;iface++)
            fprintf(pRti->fp, "%d %d\n",myBCTopo[iface].type,myBCTopo[iface].id);
            //fprintf(fp, "%d %d\n",iface,iface);

        
    }

    pwpFileClose(pRti->fp);
    return ret;
}

static PWP_BOOL
writeMesh(CAEP_RTITEM *pRti, PWGM_HBLOCK block, PWP_UINT32 blkidx,PWP_UINT32 bfsize)
{
    PWP_BOOL ret = PWP_FALSE;
    PWGM_STR_SIZE size;
    PWGM_BLOCKDATA blkData;
    PWGM_VERTDATA vertData;
    PWGM_INDEX3 ijk;
    PWP_UINT32 temp;

    char filename[25];

    if (pRti && PWGM_HBLOCK_ISVALID(block) && PwBlkSize(block, &size)) 
    {
        sprintf(filename,"mesh/mesh_%d.x\0",blkidx);        
        pRti->fp = pwpFileOpen(filename,(sysFILEMODE)(pwpWrite | pwpAscii)); 

        fprintf(pRti->fp, "%d %d %d %d\n",size.i,size.j,size.k,bfsize);

        for (ijk.k=0; (ijk.k < size.k); ijk.k++) 
        {
            for (ijk.j=0; (ijk.j < size.j); ijk.j++) 
            {
                for (ijk.i=0; (ijk.i < size.i); ijk.i++) 
                {
                    ret=PwBlkNdxVertData(block, ijk, &vertData);
                    fprintf(pRti->fp, "%20.14f ",vertData.x);
                }
            }
        }

        for (ijk.k=0; (ijk.k < size.k); ijk.k++) 
        {
            for (ijk.j=0; (ijk.j < size.j); ijk.j++) 
            {
                for (ijk.i=0; (ijk.i < size.i); ijk.i++) 
                {
                    ret=PwBlkNdxVertData(block, ijk, &vertData);
                    fprintf(pRti->fp, "%20.14f ",vertData.y);
                }
            }
        }

        for (ijk.k=0; (ijk.k < size.k); ijk.k++) 
        {
            for (ijk.j=0; (ijk.j < size.j); ijk.j++) 
            {
                for (ijk.i=0; (ijk.i < size.i); ijk.i++) 
                {
                    ret=PwBlkNdxVertData(block, ijk, &vertData);
                    fprintf(pRti->fp, "%20.14f ",vertData.z);
                }
            }
        }
    }

    pwpFileClose(pRti->fp);
    return ret;
}

static void
writeBlock(CAEP_RTITEM *pRti,int bfsize)
{
    PWP_UINT32 block_num = PwModBlockCount(pRti->model);    
    PWP_UINT32 block_idx = 0;
    PWGM_HBLOCK block ;
    PWGM_STR_SIZE size;
    system("mkdir mesh");
    system("mkdir bc_topo");
    if (caeuProgressBeginStep(pRti, block_num)) {
        while (block_idx < block_num) {
            block = PwModEnumBlocks(pRti->model,block_idx);
            writeMesh(pRti,block,block_idx,bfsize);
            writeTopo(pRti,block,block_idx);            
            caeuProgressIncr(pRti);
            block_idx++;
        }
        caeuProgressEndStep(pRti);
    }
}

/**************************************/
PWP_BOOL
runtimeWrite(CAEP_RTITEM *pRti, PWGM_HGRIDMODEL model,
             const CAEP_WRITEINFO *pWriteInfo)
{
    PWP_BOOL ret = PWP_FALSE;

    if (pRti && model && pWriteInfo) {
        PWP_UINT32 cnt = 1; /* the # of MAJOR progress steps */

        if (caeuProgressInit(pRti, cnt)) {
            PWP_UINT32 bfsize;
            PwModGetAttributeUINT32(model, "buffer size", &bfsize);
            writeBlock(pRti,bfsize);
            caeuProgressEnd(pRti, ret);
            ret = !pRti->opAborted;
        }
    }
    return ret;
}


PWP_BOOL
runtimeCreate(CAEP_RTITEM * /*pRti*/)
{
    PWP_BOOL ret = PWP_TRUE;

    // Uncomment this INFO attribute if your solver supports both big and
    // little endian byte orderings.
    //ret = caeuAssignInfoValue("AllowedFileByteOrders",
    //        "BigEndian|LittleEndian", true);

    // Uncomment one of these INFO attributes if your solver requires a
    // particular byte ordering.
    //ret = caeuAssignInfoValue("AllowedFileByteOrders", "BigEndian", true);
    //ret = caeuAssignInfoValue("AllowedFileByteOrders", "LittleEndian", true);

    // These attributes are for example only. You can publish any attribute
    // needed for your solver.
    ret = ret &&
          caeuPublishValueDefinition("buffer size", PWP_VALTYPE_UINT, "3",
              "RW", "Buffer layer weith", "3 5");
    //ret = ret &&
    //      caeuPublishValueDefinition("iterations", PWP_VALTYPE_UINT, "5",
    //          "RW", "Number of iterations", "0 2000") &&
    //      caeuPublishValueDefinition("magnitude", PWP_VALTYPE_INT, "-5",
    //          "RW", "Signed int magnitude", "-100 100") &&
    //      caeuPublishValueDefinition("mach", PWP_VALTYPE_REAL, "0.3", "RW",
    //          "Incoming flow velocity", "-Inf +Inf 0.0 50.0") &&
    //      caeuPublishValueDefinition("temperature", PWP_VALTYPE_REAL, "77.5",
    //          "RW", "Ambient temperature", "-Inf +Inf -100.0 3000.0") &&
    //      caeuPublishValueDefinition("temperature.units", PWP_VALTYPE_ENUM,
    //          "Fahrenheit", "RW", "Grid temperature units", TempUnitEnum) &&
    //      caeuPublishValueDefinition("units", PWP_VALTYPE_ENUM, "Inches",
    //          "RW", "Grid dimensional units", DimUnitEnum) &&
    //      caeuPublishValueDefinition("description", PWP_VALTYPE_STRING, "",
    //          "RW", "Grid description", "") &&
    //      caeuPublishValueDefinition("linear", PWP_VALTYPE_BOOL, "reject",
    //          "RW", "Grid is linear", "reject|accept");

    return ret;
}


PWP_VOID
runtimeDestroy(CAEP_RTITEM * /*pRti*/)
{
}
