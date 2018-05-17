package zmq_forwarder;

import org.zeromq.ZMQ;
import org.zeromq.ZMQ.Context;
import org.zeromq.ZMQ.Socket;

public class Subscriber {
	public static void main(String[] args) {
		Context context = ZMQ.context(1);
		Socket subscriber = context.socket(ZMQ.SUB);

		subscriber.connect("tcp://127.0.0.1:8888");
		subscriber.subscribe("1");

		try {
			while (!Thread.currentThread().isInterrupted()) {
				String topic = subscriber.recvStr();
				String payload = subscriber.recvStr();
				System.out.println(topic + " : " + payload);
			}
		} catch (Exception e) {
			System.err.println(e.getMessage());
		} finally {
			subscriber.close();
			context.term();
		}

	}
}
