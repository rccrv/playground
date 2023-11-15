package com.github.com.rccrv.playground.algorithms;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Random;

public class Quicksort {
    record Pair<F, S> (F first, S second) {
    }

    static Pair<List<Integer>, List<Integer>> part(List<Integer> v) {
        int pivot = v.size() - 1;
        int j = 0;

        for (int i = 0; i < v.size(); i++) {
            if (v.get(i) < v.get(pivot)) {
                Collections.swap(v, i, j);
                j++;
            }
        }

        Collections.swap(v, j, pivot);
        var n1 = v.subList(0, j);
        var n2 = v.subList(j + 1, v.size());
        var r = new Pair<List<Integer>, List<Integer>>(n1, n2);

        return r;
    }

    static void quicksort(List<Integer> v) {
        if (v.size() > 1) {
            var p = part(v);
            quicksort(p.first());
            quicksort(p.second());
        }
    }

    static void printArrayList(ArrayList<Integer> v) {
        System.out.print("[");
        for (int i = 0; i < v.size(); i++) {
            System.out.format("%d%s", v.get(i), (i + 1 < v.size() ? ", " : ""));
        }
        System.out.print("]\n");
    }

    public static void main(String[] args) {
        var rand = new Random();

        var distrib_size = rand.nextInt(40) + 10;
        var v = new ArrayList<Integer>();
        for (int i = 0; i < distrib_size; i++) {
            v.add(rand.nextInt(100) + 1);
        }

        printArrayList(v);
        quicksort(v);
        printArrayList(v);
    }
}
