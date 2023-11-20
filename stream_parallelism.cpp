//Apach cloud streaming
int main() {
    public class EventFilterBolt extends BaseRichBolt { 
        public void execute(Tuple tuple) {
        if(tuple.getStringByField("event_type").equals("view")) {
            _collector.emit(tuple, tuple.getValues());
        }
    _collector.ack(tuple);
  }
}
}

//C1 - C2 - C3 - C4
//the code above check with tuple whether the request must be done on the core 
//most streaming parallelism is done in the cloud for larger hardware and control