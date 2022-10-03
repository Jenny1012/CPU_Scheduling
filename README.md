# CPU_Scheduling  
一、	開發環境  
Dev C++  
  
二、	實作方法與流程  
FCFS: 預先將所有的process依照抵達時間小到大排好序(若抵達時間相同則按照ID小到大排序)，再一個一個依照順序將process完整執行至結束  
  
RR: 預先將所有的process 依照抵達時間小到大排好序，然後取第一個process塞入queue中，每次做RR則取queue中的第一個process做排程，若此process還沒做完則放到queue的最後面。而每做完一個process則要去檢查再剛剛做完process的處理時間(time slice)裡面有沒有新到達的process，並同時標記此新process進入queue中(若是剛剛好在處理完process之後抵達的new process，要讓它先進入queue中)，再一個一個依照queue中的順序輪替直到結束。  
  
SRTF: 預先將所有的process 依照抵達時間小到大排好序，並找出在已抵達的process中的最小剩餘執行時間的process即可。先從排好序的process中找出最小剩餘執行時間的process，做完之後要隨時檢查是否有新進的process且找出最小剩餘執行時間的process，這樣繼續做到所有process都做完。  
  
PPRR: 預先將所有的process 依照priority小到大排好序，並將已抵達的process塞到待處理process的陣列中，同時在陣列中按照priority小到大排好順序，每次執行只做待處理process的陣列中的第一個process，並同時記錄此process為前一個執行的process，並塞到他該輪的位置(若是陣列中有相同priority的其他process，則放在同priority process的最後一位；若在陣列中沒有相同priority的process，則找到陣列中比目前priority大的位置的前一位放)，並繼續做下一個process。而在執行下一個process時檢查前一個做完的process是否為同priority process(需要做Round Robin)，若是相同，則將上一個process跟目前process的位置對調並執行；若否，則繼續執行目前的process。每做完一次都會檢查有沒有新進來的process，並放進待處理process的陣列中(放在該放的位置，判斷方式如上)。  
  
HRRN: 預先將所有的process 依照抵達時間小到大排好序，並找出在已抵達的process中的最大HRRN ratio的process即可。先從排好序的process中找出最大HRRN ratio的process，做完之後要隨時檢查是否有新進的process且找出最大HRRN ratio的process，這樣繼續做到所有process都做完。  
  

最後在每個排程每做完一個process的時候計算其turnaround與waiting時間。  
  

三、	不同排程法的比較  
|-| FCFS | RR |	SRTF | PPRR | HRRN |  
|-|------|----|------|------|------|  
| Waiting (1) |	14.3333 |	18.4	| 8.06667 | 14.667 | 11.6 |  
| Turnaround (1) |18.2 | 22.2667 |	11.9333 |	18.5333 |	15.4667 |  
| Waiting (2)	| 8.4 |	6.4	| 3 |	9.4 | 8.2 |  
| Turnaround (2) | 13.2 |	11.2 | 7.8 | 14.2 | 13 |  
| Waiting (3) |	6.66667 | 11.6667 | 6.6667 | 12.5 | 6.6667 |  
| Turnaround (2) | 24.1667 | 29.1667 | 24.1667 | 30 | 24.1667 |  
   
(紅色 : 最長時間，藍色 : 最短時間)    
  
  
  

RR與PPRR所需要的等待時間比其他三種排程法的等待時間較久，因為此兩種方法是將相同元素(同arrival time或同priority)的process輪替直到做完，當過程中有遇到新的、可以奪取目前輪替的process後則進行奪取，所以會導致早到的process被拖延、每個process的turnaround時間被拉長。  
  
SRTF是以等待時間做評估的最佳演算法法，因為是選取目前已經抵達的process所剩餘的最小時間優先做，所以平均等待時間跟平均turnaround時間都會是最短的。  
  
FCFS的平均等待時間與平均turnaround都較長，因為當有process的CPU burst較長且較早抵達的時候，會拖延到之後抵達的process，所以之後所有的process的等待時間都會被延長，等待時間被延長turnaround時間也會被延長。  
  
而HRRN是採取變動優先等級的方式，滾動調整優先等級，所以不會讓CPU burst長的process永遠拿不到執行權，但是因為是non preemptive，所以會萬一有早抵達但CPU burst較長的process出現，會使得所有process平均等待時間與平均turnaround時間會較長。  
  
四、	結果與討論  
SRTF的結果為最符合理想CPU排程的狀態，平均等待時間與平均turnaround時間都會是最短的，但是此排程會造成starvation，執行時間較久的process有可能會永遠拿不到執行權。而RR所需要的等待時間雖然比其他四種排程法的等待時間較久，但是不會造成starvation，因為此排程會輪流執行已經抵達的process，所以不會造成process無限延宕的情形。HRRN與FCFS也不會造成starvation，因為HRRN會滾動式調整優先等級，response ratio高的優先處理，而FCFS會依抵達時間依序執行。PPRR有機會會造成starvation，萬一之後有晚抵達但priority較小的process，則會不斷插隊執行，所以可能會造成早抵達但priority大的process無限延宕的情形。  

