library(ggplot2)
library(dplyr)
library(tidyr)
library(hrbrthemes)

args=commandArgs(trailingOnly=TRUE)
df_var = read.csv(args[1], header=F, strip.white=T)
names(df_var) = c("Container", "Resource_Name", "Type", "Start", "End", "Duration", "Value"); 
df_var = df_var[!(names(df_var) %in% c("Container"))]
df_var <- df_var %>% filter(Type == "bandwidth_used") %>% filter(Resource_Name != "loopback")
df_var = df_var %>% mutate(Resource_Name = as.character(Resource_Name))
df_var = df_var %>% mutate(Percentage = Value/1000000000)
df_var = df_var %>% mutate(Amount_data = Value*Duration)
split_df <- split(df_var, df_var$Resource_Name, drop=TRUE)

df_avgbandwidth = data.frame(matrix(ncol = 2, nrow = 0))
names(df_avgbandwidth) <- c("Resource_Name", "AvgBandwidth")
for (i in 1:length(split_df))
{
    df_avgbandwidth <- df_avgbandwidth %>% add_row(Resource_Name = split_df[[i]]$Resource_Name[1], AvgBandwidth = sum(split_df[[i]]$Amount_data)/sum(split_df[[i]]$Duration))
}
df_avgbandwidth <- df_avgbandwidth %>% mutate(Percentage = AvgBandwidth/1000000000)

df_heatmap <- as.data.frame(do.call(rbind, strsplit(df_avgbandwidth$Resource_Name, "_to_")))
names(df_heatmap) <- c('source', 'dest')
df_heatmap <- df_heatmap %>% mutate(Percentage=df_avgbandwidth$Percentage)
ggplot(df_heatmap, aes(source, dest, fill= Percentage)) + geom_tile() + scale_fill_gradient(low="white", high="blue", breaks=c(0,0.25,0.5,0.75, 1), limits=c(0,1), na.value = "white") + theme(axis.text.x = element_text(angle = 90, vjust = 0.5, hjust=1)) + ggtitle(args[1])
ggsave(paste(args[2], "-heatmap", ".png", sep=""), width = 30, height = 30, units = "cm") 

ggp <- ggplot(df_avgbandwidth, aes(x=Resource_Name, y=Percentage)) + geom_bar(stat = "identity") + theme(axis.text.x = element_text(angle = 90, vjust = 0.5, hjust=1)) + ylim(0, 1)

ggp <- ggp + geom_hline(aes(yintercept = mean(Percentage)), color="blue") + geom_text(aes(0, mean(Percentage), label = paste(round(mean(Percentage), digit=2), "+-", round(sd(Percentage), digit=2)), hjust = -1, vjust = - 1))
ggsave(paste(args[2], "-link-percentage", ".png", sep=""), width = 30, height = 30, units = "cm")

head(df_var)
str(df_var)
df_var %>% filter(Type == "bandwidth_used") %>% filter(Resource_Name != "loopback") %>%
    ggplot(aes(x=Start,y=Percentage)) + geom_step() + facet_wrap(~Resource_Name)

ggsave(paste(args[2], ".png", sep=""), width = 30, height = 30, units = "cm")


