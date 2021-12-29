// Shiritori (https://en.wikipedia.org/wiki/Shiritori) solver.

package main

import (
	"fmt"
	"sort"
	"strings"
)

type Word struct {
	Original string
	Start    string
	End      string
}

type Chain struct {
	Length int
	Words  []string
}

var resultChains []Chain

func isMatch(w1, w2 Word) bool {
	return w1.End == w2.Start
}

func buildMatches(currentChain []Word, usedIndices map[int]bool, words []Word) {
	var lastWord *Word
	if len(currentChain) > 0 {
		lastWord = &currentChain[len(currentChain)-1]
	}
	for i, w := range words {
		if _, ok := usedIndices[i]; ok {
			continue
		}
		if lastWord != nil && !isMatch(*lastWord, w) {
			continue
		}

		currentChain = append(currentChain, w)
		usedIndices[i] = true
		buildMatches(currentChain, usedIndices, words)

		if len(currentChain) >= 10 {
			chainWords := make([]string, 0, len(currentChain))
			for _, c := range currentChain {
				chainWords = append(chainWords, c.Original)
			}
			resultChains = append(resultChains, Chain{
				Length: len(currentChain),
				Words:  chainWords,
			})
		}

		currentChain = currentChain[:len(currentChain)-1]
		delete(usedIndices, i)
	}
}

func main() {
	words := []Word{
		{Original: "はんこ", Start: "は", End: "こ"},
		{Original: "まらかす", Start: "ま", End: "す"},
		{Original: "ずかん", Start: "す", End: "ん"},
		{Original: "かめ", Start: "か", End: "め"},
		{Original: "こま", Start: "こ", End: "ま"},
		{Original: "ぱぷりか", Start: "は", End: "か"},
		{Original: "れいぞうこ", Start: "れ", End: "こ"},
		{Original: "すみれ", Start: "す", End: "れ"},
		{Original: "からす", Start: "か", End: "す"},
		{Original: "りす", Start: "り", End: "す"},
		{Original: "め", Start: "め", End: "め"},
		{Original: "すいか", Start: "す", End: "か"},
		{Original: "こすもす", Start: "こ", End: "す"},
		{Original: "ごま", Start: "こ", End: "ま"},
		{Original: "らっこ", Start: "ら", End: "こ"},
		{Original: "めだか", Start: "め", End: "か"},
		{Original: "ごりら", Start: "こ", End: "ら"},
		{Original: "りんご", Start: "り", End: "こ"},
		{Original: "かん", Start: "か", End: "ん"},
		{Original: "らっぱ", Start: "ら", End: "は"},
		{Original: "めん", Start: "め", End: "ん"},
		{Original: "れもん", Start: "れ", End: "ん"},
	}

	usedIndices := make(map[int]bool)
	buildMatches([]Word{}, usedIndices, words)

	sort.Slice(resultChains, func(i, j int) bool {
		return resultChains[i].Length > resultChains[j].Length
	})

	for _, c := range resultChains {
		fmt.Printf("Len: %d, Res: %+v\n", c.Length, strings.Join(c.Words, ", "))
	}
}
