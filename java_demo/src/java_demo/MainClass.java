package java_demo;
import java.nio.ByteBuffer;
import java.nio.charset.Charset;

import io.netty.buffer.ByteBuf;
import io.netty.buffer.Unpooled;
import io.netty.util.ByteProcessor;


public class MainClass {

	public static void main(String[] args) {
		test2();

	}
	
	static public void test2()
	{
		UnpByteBuffer buffer = new UnpByteBuffer(512);
		assert(buffer.capacity() == 512);
		assert(buffer.readable() == false);
		assert(buffer.writeable() == true);
		assert(buffer.readIndex() == 0 && buffer.writeIndex() == 0);
		buffer.writeByte(97);
		assert(buffer.writeIndex() == 1);
		assert(buffer.readIndex() == 0);
		buffer.writeByte(97);
		assert(buffer.writeIndex() == 2);
		assert(buffer.readIndex() == 0);
		System.out.println(buffer.readByte());
		assert(buffer.writeIndex() == 2);
		assert(buffer.readIndex() == 1);
		System.out.println((char)buffer.readByte());
		assert(buffer.writeIndex() == 2);
		assert(buffer.readIndex() == 2);
	}
	
	static public void test1()
	{
		ByteBuffer bbuffer = ByteBuffer.allocate(128);
		byte b = 'a';
		bbuffer.put(b);
		bbuffer.put(b);
		System.out.println("capacity: " + bbuffer.capacity());
		System.out.println("arratOfSet: " + bbuffer.arrayOffset());
		System.out.println("hasarray: " + bbuffer.hasArray());
		System.out.println("position: " + bbuffer.position());
		System.out.println("limit: " + bbuffer.limit());
		System.out.println("getchar: " + bbuffer.get());//get position++
		bbuffer.put(b);//put position++
		bbuffer.flip();// limit = position, position  = 0
		System.out.println("getchar: " + bbuffer.get());//get position++
		System.out.println("getchar: " + bbuffer.get());//get position++
		
		
		System.out.println("...............");
		
		ByteBuf buf = Unpooled.buffer(128);
		System.out.println(buf.capacity());
		buf.writeInt(97);
		buf.writeByte(97);
		int i = 0;
		while(buf.readableBytes() > 2)
		{
			System.out.print(++i + ": ");
			System.out.println(buf.readChar());
		}
		
//		ByteBuf buf2 = PooledByteBufAllocator.DEFAULT.buffer(128);
		ByteBuf buf2 = Unpooled.buffer(128);
		System.out.println("capacity: " + buf2.capacity());
		buf2.writeInt(97);
		buf2.writeByte(97);
		i = 0; 
		int indexOfNotNull = buf2.forEachByte(ByteProcessor.FIND_NON_NUL);
		System.out.println("IndexOfNotNull: " + indexOfNotNull);
		
		
		ByteBuf buf3 = Unpooled.buffer(128);
		String contents = "HTTP/1.1 / GET\r\n"
				+ "HEADERS: asdasdqwdasd"
				+ "Cookies: asdfwqdkapfjosgfoiegwjep";
		buf3.writeBytes(contents.getBytes());
		int indexOfCRLF = buf3.forEachByte(ByteProcessor.FIND_CRLF);
		System.out.println("IndexOfCRLF: " + indexOfCRLF);
		if(buf3.readableBytes() > indexOfCRLF)
		{
			buf2.clear();
			buf3.readBytes(buf2, indexOfCRLF + 2);
			buf3.discardReadBytes();
			System.out.println(buf2.toString(0, buf2.writerIndex(), Charset.defaultCharset()));
		}
		System.out.println((char)buf3.readByte());
		System.out.println((char)buf3.readByte());
		System.out.println((char)buf3.readByte());


		System.out.println("IndexOfCRLF: " + indexOfCRLF);
		
	}
}
