using System;
using Microsoft.SPOT;

namespace Samraksh.APPS.ActiveTag
{

    public enum NodeStatus
    {
        Active,
        InActive,
    }

    public class Node
    {
        UInt16 _macaddress;
        MessageAuthorFunctionType _function;
        UInt16 _alarmCount;
        NodeStatus _status;

        public NodeStatus status
        {
            get { return this._status; }
            set { this._status = value; }
        }

        public UInt16 macaddress
        {
            get { return this._macaddress; }
            set { this._macaddress = value; }
        }

        public MessageAuthorFunctionType function
        {
            get { return this._function; }
            set { this._function = value; }
        }

        public UInt16 alarmCount
        {
            get { return this._alarmCount; }
            set { this._alarmCount = value; }
        }

        public Node()
        {
            macaddress = 0;
            function = MessageAuthorFunctionType.FoodNodeFcn;
            alarmCount = 0;
        }

        public Node(UInt16 macaddress, MessageAuthorFunctionType function, UInt16 alarmCount)
        {
            this.macaddress = macaddress;
            this.function = function;
            this.alarmCount = alarmCount;
        }

    }

    public class NetworkNodeList
    {
    
        const int MAXNODES = 32;

        Node[] nodeList = new Node[MAXNODES];

        UInt16 currNodeIndex = 0;

        public NetworkNodeList()
        {
            for (int i = 0; i < MAXNODES; i++)
            {
                nodeList[i] = new Node();
                nodeList[i].macaddress = 0;
            }
        }

        public UInt16 getNumberOfMembers()
        {
            return currNodeIndex;
        }

        public void Add(UInt16 macaddress)
        {

            if (macaddress == 0)
                return;

            bool exists = false;

            for(int i = 0; i < MAXNODES; i++)
            {
                if (nodeList[i].macaddress == macaddress && macaddress != 0)
                {
                    exists = true;
                }
            }

            if (!exists)
            {
                nodeList[currNodeIndex++].macaddress = macaddress;
            }
        }

        public Node[] GetList()
        {
            return nodeList;
        }

        public void UpdateNodeStats(UInt16 macaddress, MessageAuthorFunctionType function, NodeStatus status)
        {
            bool exists = false;

            if (macaddress == 0)
                return;

            int nodeIndex = currNodeIndex;

            for (int i = 0; i < MAXNODES; i++)
            {
                if (nodeList[i].macaddress == macaddress)
                {
                    nodeList[i].status = status;
                    nodeList[i].function = function;
                    nodeList[i].alarmCount++;
                    exists = true;
                }
            }
            if (!exists)
            {
                nodeList[nodeIndex].macaddress = macaddress;
                nodeList[nodeIndex].status = status;
                nodeList[nodeIndex].function = function;
                nodeList[nodeIndex].alarmCount++;
                currNodeIndex++;
            }

        }
    }
}
