const {
  Worker,
  isMainThread,
  BroadcastChannel,
  workerData,
} = require("worker_threads");

const bc = new BroadcastChannel("001");

const sleep = (s) => new Promise((r) => setTimeout(r, s * 1000));

const thread = async (num) => {
  console.log(`Hello from thread ${num}`);
  while (true) {
    await sleep(Math.floor(Math.random() * 5) + 1);
    bc.postMessage(num);
  }
};

const main = async () => {
  console.log(`Hello from main`);
  bc.onmessage = (e) => {
    console.log(`Received from thread ${e.data}`);
  };
  new Worker(__filename, { workerData: { wNumber: 1 } });
  new Worker(__filename, { workerData: { wNumber: 2 } });
  new Worker(__filename, { workerData: { wNumber: 3 } });
};

if (isMainThread) {
  main();
} else {
  const data = workerData;
  thread(data.wNumber);
}
