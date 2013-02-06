using System;
using Microsoft.SPOT;
using System.Collections;

namespace CircularBuffer
{
    public class Message
    {
        byte[] data;
        int waterlevel;
        int length;

        public Message()
        {
            this.data = new byte[128];
            for (int i = 0; i < 128; i++)
            {
                this.data[i] = 0;
            }
        }

        public Message(byte[] idata)
        {
            this.data = new byte[128];
            for (int i = 0; i < 128; i++)
            {
                this.data[i] = idata[i];
            }
            waterlevel = 0;
        }

        public void Clear()
        {
            for (int i = 0; i < data.Length; i++)
                data[i] = 0;
        }

        public int Length
        {
            set
            {
                this.length = value;
            }

            get
            {
                return length;
            }
        }

        public int GetFillLevel()
        {
            return waterlevel;
        }

        public byte[] GetMessage()
        {
            return data;
        }

        public void load(byte[] temp, int offset, int size)
        {
            if(waterlevel > 128)
                throw new InvalidOperationException("Overflow");

            for (int i = offset; i < size; i++)
            {
                this.data[i] = temp[i - offset];
            }

            waterlevel += size;
        }

    }

    public class CircularBuffer
    {
        private int capacity;
        private int size;
        private int head;
        private int tail;
        private Message[] buffer;
        private System.Object lockThis = new System.Object();

        public CircularBuffer(int capacity)
        {
            buffer = new Message[capacity];
            size = 0;
            this.capacity = capacity;
            head = 0;
            tail = 0;
        }

        public int Size
        {
            get { return size; }
        }

        public bool put(Message item)
        {
            lock (lockThis)
            {
                if (size == capacity)
                    return false;

                buffer[head % capacity] = item;
                size++;
                head++;

                return true;
            }

        }

        public Message Get()
        {
            lock (lockThis)
            {
                if (size == 0)
                    return null;
                    //throw new InvalidOperationException("The Buffer is empty");

                var item = buffer[tail % capacity];
                tail++;
                size--;

                return item;
            }
        }

    }
}
