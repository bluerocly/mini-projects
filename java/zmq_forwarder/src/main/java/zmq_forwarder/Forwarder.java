package zmq_forwarder;

import org.zeromq.ZMQ;
import org.zeromq.ZMQ.Context;
import org.zeromq.ZMQ.Socket;

public class Forwarder {
	public static void main(String[] args) {
		
		Context context = ZMQ.context(1);
		Socket frontend = context.socket(ZMQ.SUB);
//		frontend.bind("tcp://*:9999");
		frontend.bind("ipc://frontend.ipc");
		frontend.subscribe("".getBytes());

		Socket backend = context.socket(ZMQ.PUB);
		backend.bind("tcp://*:8888");
		
		try {
			System.out.println("Starting forwarder");
			ZMQ.proxy(frontend, backend, null);
		} catch (Exception e) {
			System.err.println(e.getMessage());
		} finally {
			frontend.close();
			backend.close();
			context.term();
		}

	}

}
