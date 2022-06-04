const quicksort = (a: number[]): number[] => {
  if (a.length > 1) {
    const p1 = a.slice(0, a.length - 1).filter((x) => x < a[a.length - 1]);
    const p2 = a.slice(0, a.length - 1).filter((x) => x >= a[a.length - 1]);
    return [...quicksort(p1), a[a.length - 1], ...quicksort(p2)];
  }

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
