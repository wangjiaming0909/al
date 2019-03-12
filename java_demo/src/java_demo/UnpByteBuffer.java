package java_demo;

public class UnpByteBuffer {
	public static final int DEFAULT_CAPACITY = 128;
	public UnpByteBuffer(int capacity) 
	{
		if(capacity <= 0)
			this.capacity_ = UnpByteBuffer.DEFAULT_CAPACITY;
		capacity_ = capacity;
		readIndex_ = writeIndex_ = size_ = 0;
		contents_ = new byte[capacity_];
	}
	
	public int capacity() { return capacity_; }
	public int readIndex() { return readIndex_;}
	public int writeIndex() { return writeIndex_;}
	public int size() { return size_;}
	public byte[] contents() { return contents_;}
	
	int writeByte(int b)
	{
		if(!writeable())
		{
			throw new IndexOutOfBoundsException();
		}
		contents_[writeIndex_] = (byte)b;
		writeIndex_++;
		return 1;
	}
	
	public byte readByte()
	{
		if(!readable())
		{
			throw new IndexOutOfBoundsException();
		}
		byte ret;
		ret = contents_[readIndex_];
		readIndex_++;
		return ret;
	}
	
	public boolean writeable()
	{
		return writeIndex_ < capacity_;
	}
	public boolean readable()
	{
		return readIndex_ < writeIndex_;
	}
	
	private int capacity_;
	private int readIndex_;
	private int writeIndex_;
	private int size_;
	private byte[] contents_;
}
