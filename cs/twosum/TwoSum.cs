using System.Collections;
using System.Collections.Generic;
using System.Linq;
/*
给定一个整数数组和一个目标值，找出数组中和为目标值的两个数。
你可以假设每个输入只对应一种答案，且同样的元素不能被重复利用。
示例:
给定 nums = [2, 7, 11, 15], target = 9
因为 nums[0] + nums[1] = 2 + 7 = 9
所以返回 [0, 1]
 */

public class TwoSum{

    //just return the two number
    public static int[] SumOfTwo(int[] source, int target){
        int[] result = new int[2];
        HashSet<int> set = new HashSet<int>();
        IEnumerable<int> arr = source.Distinct();
        foreach (var item in arr)
            set.Add(item);
        foreach (var item in arr) {
            if(set.Contains(target - item)){
                result[0] = item;
                result[1] = target - item;
            }
        }
        return result;
    }

    //return the index of the two number
    static public int[] SumOfTwo2(int[] source, int target){
        int[] result = new int[2];
        Dictionary<int, int> dic = new Dictionary<int, int>();
        for (int i = 0; i < source.Length; i++){
            dic.Add(i, source[i]);
        }
        IEnumerable<int> arr = source.Distinct();
        foreach (var i in arr){
            if(dic.Values.Contains(target - i)){
            }
        }
        return result;
    }
}