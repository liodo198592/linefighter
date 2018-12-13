package common.commu.handler;

import java.util.List;

import org.apache.mina.core.buffer.IoBuffer;
import org.apache.mina.core.session.IoSession;
import org.apache.mina.filter.codec.CumulativeProtocolDecoder;
import org.apache.mina.filter.codec.ProtocolDecoderOutput;

import common.commu.MessageGson;
import common.message.AbstractMessage;

public class MessageDecoder extends CumulativeProtocolDecoder {

	@Override
	protected boolean doDecode(IoSession session, IoBuffer in,
			ProtocolDecoderOutput out) throws Exception {
		// TODO Auto-generated method stub
		
		int length = in.remaining();
		
		byte[] data  = new byte[length];
		in.get(data, 0, length);
		
		String strMsg = new String(data);
		
		MessageGson messageGson = new MessageGson();
		messageGson.fromJson(strMsg);
		
		List<AbstractMessage> messageList = messageGson.getMessageList();
		
		for(AbstractMessage message:messageList){
			out.write(message);
		}
		
		
		
		return true;
	}

}
