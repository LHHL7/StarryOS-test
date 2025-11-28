#[cfg(test)]
// 测试数据结构以及方法的测试模块

mod tests {
    use alloc::sync::Arc;

    use axsync::Mutex;
    use linux_raw_sys::general::__kernel_size_t;

    use super::*;

    #[test]
    fn test_msqid_ds_creation() {
        let ds = MsqidDs::new(123, 0o666, 1000, 1000, 1000);
        assert_eq!(ds.msg_perm.key, 123);
        assert_eq!(ds.msg_perm.mode, 0o666);
        assert_eq!(ds.msg_qbytes, MSGMNB as __kernel_size_t);
    }

    #[test]
    fn test_message_lifecycle() {
        let mut inner = MsgInner::new(123, 1, 0o666, 1000, 1000, 1000);

        // test enqueue
        assert!(inner.enqueue_message(1, b"test").is_ok());
        assert_eq!(inner.msqid_ds.msg_qnum, 1);

        // test find (without removing)
        let msg = inner.find_message_by_type(1);
        assert!(msg.is_some());
        assert_eq!(inner.msqid_ds.msg_qnum, 1); // count unchanged

        // test removal
        if let Some(msg) = inner.remove_first_message_of_type(1) {
            assert_eq!(msg.data, b"test");
            assert_eq!(inner.msqid_ds.msg_qnum, 0); // count decreased
        }
    }

    #[test]
    fn test_find_message_scenarios() {
        let mut inner = MsgInner::new(123, 1, 0o666, 1000, 1000, 1000);

        // Prepare test data
        inner.enqueue_message(1, b"type1").unwrap();
        inner.enqueue_message(2, b"type2").unwrap();
        inner.enqueue_message(3, b"type3").unwrap();

        // Test various find scenarios
        assert!(inner.find_first_message().is_some()); // first message
        assert!(inner.find_message_by_type(2).is_some()); // specified type
        assert!(inner.find_message_not_equal(1).is_some()); // not equal to 1
        assert!(inner.find_message_less_equal(2).is_some()); // smallest type ≤ 2
    }
}
