<<<<<<< HEAD
const part = (a: number[]): [number[], number[]] => {
  const pivot = a.length - 1;
  let j = 0;

  for (let i = 0; i < a.length; i++) {
    if (a[i] < a[pivot]) {
      [a[i], a[j]] = [a[j], a[i]];
      j++;
    }
  }

  [a[j], a[pivot]] = [a[pivot], a[j]];

  return [a.splice(0, j), a.splice(1)];
};

const quicksort = (a: number[]): number[] => {
  if (a.length > 1) {
    let [p1, p2] = part(a);

    let r1 = quicksort(p1);
    let r2 = quicksort(p2);

    let r = [...r1, a[0], ...r2];
    return r;
  }
=======
const quicksort = (a: number[]): number[] => {
  if (a.length > 1) {
    const p1 = a.slice(0, a.length - 1).filter((x) => x < a[a.length - 1]);
    const p2 = a.slice(0, a.length - 1).filter((x) => x >= a[a.length - 1]);
    return [...quicksort(p1), a[a.length - 1], ...quicksort(p2)];
  }

>>>>>>> f5ace1ffc3454fa9b64d8d60a049f19654fbe565
  return a;
};

const main = () => {
  let a = [];

  const n = Math.floor(Math.random() * 41 + 10);
  for (let i = 0; i < n; i++) {
    a.push(Math.floor(Math.random() * 101 + 1));
  }

  console.log(a);
  a = quicksort(a);
  console.log(a);
};

main();
