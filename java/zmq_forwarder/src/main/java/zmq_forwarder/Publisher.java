package zmq_forwarder;

import java.util.Random;

import org.zeromq.ZMQ;
import org.zeromq.ZMQ.Context;
import org.zeromq.ZMQ.Socket;

public class Publisher {
	public static void main(String[] args) throws Exception {
		Context context = ZMQ.context(1);
		Socket publisher = context.socket(ZMQ.PUB);

		Random rand = new Random(System.currentTimeMillis());
		int serverNo = rand.nextInt(10000);

//		publisher.connect("tcp://127.0.0.1:9999");
		
		publisher.connect("ipc://frontend.ipc");

		System.out.println(String.format("Server : %s", serverNo));

		try {
			while (!Thread.currentThread().isInterrupted()) {
				String topic = String.format("%s", rand.nextInt(10));
				String payload = String.format("Server#%s", serverNo);
				publisher.sendMore(topic);
				publisher.send(payload);
				System.out.println("Sending: " + payload + " on Channel " + topic);
				Thread.sleep(250);
			}
		} catch (Exception e) {
			System.err.println(e.getMessage());
		} finally {
			publisher.close();
			context.term();
		}
	}
}
