package java_demo;
import io.netty.buffer.ByteBuf;
import io.netty.buffer.Unpooled;

public class MainClass {

	public static void main(String[] args) {
		ByteBuf buf = Unpooled.buffer();
		System.out.println(buf.capacity());
		buf.writeInt(1);
		int i = 0;
		while(buf.readableBytes() > 0)
		{
			System.out.println(++i);
			System.out.println(buf.readChar());
		}
	}
}
