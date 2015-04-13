using System;

namespace Solution
{
    /*
        Distribute hospitals among cities to minimize patient density.
        
        http://stackoverflow.com/questions/29223488/what-algorithm-is-this-best-way-to-distribute-limited-resources
        
        mcs hospitals_in_cities.cs && mono hospitals_in_cities.exe
     */
    public class Solution
    {
        static int FindBestDistribution(int totalHospitalCount, int[] populations)
        {
            // Assume at least 1 hospital for each city.
            if (totalHospitalCount < populations.Length) {
                return -1;
            }
            
            int[] hospitals = new int[populations.Length];
            float[] densities = Array.ConvertAll(populations, element => (float)element);
            
            // Put 1 hospital in each city.
            for (int i = 0; i < hospitals.Length; i++) {
                hospitals[i] = 1;
            }
            totalHospitalCount -= hospitals.Length;
            
            while (totalHospitalCount > 0) {
                int idx = GetMaxDensityIndex(densities);
                int hospitalCount = hospitals[idx] + 1;
                hospitals[idx] = hospitalCount;
                densities[idx] = (float)populations[idx] / hospitalCount;
                --totalHospitalCount;
            }
            
            return (int)Math.Ceiling(densities[GetMaxDensityIndex(densities)]);
        }
        
        static int GetMaxDensityIndex(float[] densities)
        {
            int maxIndex = -1;
            float maxValue = float.MinValue;
            for (int i = 0; i < densities.Length; i++) {
                if (densities[i] > maxValue) {
                    maxIndex = i;
                    maxValue = densities[i];
                }
            }
            return maxIndex;
        }
                
        private static int _count = 1;
        static void runCase(int hospitalCount, int[] populations, int expectedMinimum)
        {
            int actualMinimum = FindBestDistribution(hospitalCount, populations);
            if (expectedMinimum == actualMinimum) {
                Console.WriteLine("{0}: Success!", _count);
            } else {
                Console.WriteLine("{0}: Failure! {1} != {2}", _count, expectedMinimum, actualMinimum);
            }
            ++_count;
        }

        public static void Main(string[] args)
        {
            runCase(7, new int[] {200000, 500000}, 100000);
            runCase(3, new int[] {3698173, 1435143}, 1849087);
            runCase(38, new int[] {962725, 744473, 1883667, 3980182, 4566733, 2433589, 1347670, 3346447, 2831489, 272829}, 669290);
            runCase(76, new int[] {2261427, 359149, 4820519, 703064, 2787085, 2822378, 1669172, 1378943, 3896068, 1359914, 4031467, 2810237, 212899, 3297550, 3266279}, 535614);
        }
    }
}
