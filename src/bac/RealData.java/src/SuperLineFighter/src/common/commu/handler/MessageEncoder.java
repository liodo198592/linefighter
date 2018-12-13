package common.commu.handler;

import org.apache.mina.core.buffer.IoBuffer;
import org.apache.mina.core.session.IoSession;
import org.apache.mina.filter.codec.ProtocolEncoder;
import org.apache.mina.filter.codec.ProtocolEncoderOutput;


import common.commu.MessageGson;
import common.message.AbstractMessage;

public class MessageEncoder implements ProtocolEncoder {

	@Override
	public void dispose(IoSession arg0) throws Exception {
		// TODO Auto-generated method stub

	}

	@Override
	public void encode(IoSession session, Object message, ProtocolEncoderOutput out)
			throws Exception {
		// TODO Auto-generated method stub
		
		if(!(message instanceof AbstractMessage)){
			return;
		}
		
		AbstractMessage abstractMessage = (AbstractMessage)message;
		
		MessageGson messageGson = new MessageGson();
		messageGson.addMessage(abstractMessage);
		
		String strMessage = messageGson.toJson();
		
		byte[] data = strMessage.getBytes();
		
		IoBuffer buffer = IoBuffer.allocate(data.length, false);
		buffer.put(data);
		buffer.flip();
		out.write(buffer);

	}

}
