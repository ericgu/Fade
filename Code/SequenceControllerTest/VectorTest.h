#pragma once

#include "Vector.h"

class VectorTest
{
    static void TestProviderReturnsItem()
    {
        VectorDataItemProvider dataItemProvider;

        Assert::AreEqual(1, dataItemProvider.GetDataItem() != 0);
        Assert::AreEqual(1, dataItemProvider.GetInUseCount());
    }

    static void TestProviderReturnsTwoItems()
    {
        VectorDataItemProvider dataItemProvider;

        Assert::AreEqual(1, dataItemProvider.GetDataItem() != 0);
        Assert::AreEqual(1, dataItemProvider.GetDataItem() != 0);
        Assert::AreEqual(2, dataItemProvider.GetInUseCount());
    }


    static void TestProviderCanReallocateAReleasedItem()
    {
        int temp = Environment.VectorItemChunkSize;
        Environment.VectorItemChunkSize = 2;
        VectorDataItemProvider dataItemProvider;

        VectorDataItem* pDataItem1 = dataItemProvider.GetDataItem();
        VectorDataItem* pDataItem2 = dataItemProvider.GetDataItem();
        Assert::AreEqual(2, dataItemProvider.GetInUseCount());

        dataItemProvider.ReleaseDataItem(pDataItem1);

        Assert::AreEqual(1, dataItemProvider.GetInUseCount());

        VectorDataItem* pDataItem3 = dataItemProvider.GetDataItem();
        Assert::AreEqual(pDataItem1, pDataItem3);
        Assert::AreEqual(2, dataItemProvider.GetInUseCount());

        Environment.VectorItemChunkSize = temp;
    }

    static void TestProviderCanReallocateTwoReleasedItems()
    {
        int temp = Environment.VectorItemChunkSize;
        Environment.VectorItemChunkSize = 7;
        VectorDataItemProvider dataItemProvider;

        VectorDataItem* pDataItem1 = dataItemProvider.GetDataItem();
        VectorDataItem* pDataItem2 = dataItemProvider.GetDataItem();
        VectorDataItem* pDataItem3 = dataItemProvider.GetDataItem();
        VectorDataItem* pDataItem4 = dataItemProvider.GetDataItem();
        VectorDataItem* pDataItem5 = dataItemProvider.GetDataItem();
        VectorDataItem* pDataItem6 = dataItemProvider.GetDataItem();
        VectorDataItem* pDataItem7 = dataItemProvider.GetDataItem();
        Assert::AreEqual(7, dataItemProvider.GetInUseCount());

        dataItemProvider.ReleaseDataItem(pDataItem3);
        dataItemProvider.ReleaseDataItem(pDataItem5);

        Assert::AreEqual(5, dataItemProvider.GetInUseCount());

        VectorDataItem* pDataItem8 = dataItemProvider.GetDataItem();
        Assert::AreEqual(pDataItem3, pDataItem3);
        Assert::AreEqual(6, dataItemProvider.GetInUseCount());

        VectorDataItem* pDataItem9 = dataItemProvider.GetDataItem();
        Assert::AreEqual(pDataItem9, pDataItem3);
        Assert::AreEqual(7, dataItemProvider.GetInUseCount());
        Environment.VectorItemChunkSize = temp;
    }


    static void TestCreateAndSetOneItem()
    {
        Vector vector;

        vector.SetItem(0, 15.0F);

        Assert::AreEqual(15.0F, vector.GetItem(0));
        Assert::AreEqual(1, vector.GetItemCount());
    }


    static void TestCreateAndSetTwoItems()
    {
        Vector vector;

        vector.SetItem(0, 15.0F);

        Assert::AreEqual(15.0F, vector.GetItem(0));
        Assert::AreEqual(1, vector.GetItemCount());

        vector.SetItem(1, 25.0F);

        Assert::AreEqual(25.0F, vector.GetItem(1));
        Assert::AreEqual(2, vector.GetItemCount());
    }

    static void TestDestruct()
    {
        Vector::RestartVectorDataProvider();
        {
            Vector v; // force creation of data provider...
        }

        Assert::AreEqual(0, Vector::_pVectorDataItemProvider->GetInUseCount());
        {
            Vector vector;

            for (int i = 0; i < 10; i++)
            {
                vector.SetItem(i, (float)(i * i));
            }

            Assert::AreEqual(10, Vector::_pVectorDataItemProvider->GetInUseCount());
        }
        Assert::AreEqual(0, Vector::_pVectorDataItemProvider->GetInUseCount());
    }

    static void TestAddMany()
    {
        Vector vector;

        vector.SetItem(0, 15.0F);

        Assert::AreEqual(15.0F, vector.GetItem(0));
        Assert::AreEqual(1, vector.GetItemCount());

        vector.SetItem(9, 25.0F);

        Assert::AreEqual(25.0F, vector.GetItem(9));
        Assert::AreEqual(10, vector.GetItemCount());

        Assert::AreEqual(1, isnan(vector.GetItem(5)));
    }

    static void TestRandomAccess()
    {
        Vector vector;

        for (int i = 0; i < 10; i++)
        {
            vector.SetItem(i, (float)(i * i));
        }

        Assert::AreEqual(81.0F, vector.GetItem(9));

        Assert::AreEqual(9.0F, vector.GetItem(3));
        Assert::AreEqual(4.0F, vector.GetItem(2));
        Assert::AreEqual(36.0F, vector.GetItem(6));
    }

public:
    static void Run()
    {
        TestRandomAccess();
        TestCreateAndSetOneItem();
        TestCreateAndSetTwoItems();
        TestDestruct();
        TestAddMany();

        TestProviderReturnsItem();
        TestProviderReturnsTwoItems();
        TestProviderCanReallocateAReleasedItem();
        TestProviderCanReallocateTwoReleasedItems();
    }
};


